//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_LOCALE_SOURCE
#include <boost/locale/collator.hpp>
#include <boost/locale/generator.hpp>
#include "boost/locale/icu/all_generator.hpp"
#include "boost/locale/icu/cdata.hpp"
#include "boost/locale/icu/icu_util.hpp"
#include "boost/locale/icu/uconv.hpp"
#include "boost/locale/shared/mo_hash.hpp"
#include <boost/thread.hpp>
#include <vector>
#include <limits>
#include <unicode/coll.h>
#if BOOST_LOCALE_ICU_VERSION >= 402
#  define BOOST_LOCALE_WITH_STRINGPIECE 1
#  include <unicode/stringpiece.h>
#else
#  define BOOST_LOCALE_WITH_STRINGPIECE 0
#endif

#ifdef BOOST_MSVC
#pragma warning(disable:4244) // 'argument' : conversion from 'int'
#pragma warning(disable:4267) // 'argument' : conversion from 'size_t'
#endif

namespace boost {
    namespace locale {
        namespace impl_icu {
            template<typename CharType>
            class collate_impl : public collator<CharType>
            {
            public:
                typedef typename collator<CharType>::level_type level_type;
                level_type limit(level_type level) const
                {
                    if(level < 0)
                        level=collator_base::primary;
                    else if(level >= level_count)
                        level = static_cast<level_type>(level_count - 1);
                    return level;
                }

                #if BOOST_LOCALE_WITH_STRINGPIECE
                int do_utf8_compare(    level_type level,
                                        char const *b1,char const *e1,
                                        char const *b2,char const *e2,
                                        UErrorCode &status) const
                {
                    icu::StringPiece left (b1,e1-b1);
                    icu::StringPiece right(b2,e2-b2);
                    return get_collator(level)->compareUTF8(left,right,status);

                }
                #endif

                int do_ustring_compare( level_type level,
                                        CharType const *b1,CharType const *e1,
                                        CharType const *b2,CharType const *e2,
                                        UErrorCode &status) const
                {
                    icu::UnicodeString left=cvt_.icu(b1,e1);
                    icu::UnicodeString right=cvt_.icu(b2,e2);
                    return get_collator(level)->compare(left,right,status);
                }

                int do_real_compare(level_type level,
                                    CharType const *b1,CharType const *e1,
                                    CharType const *b2,CharType const *e2,
                                    UErrorCode &status) const
                {
                    return do_ustring_compare(level,b1,e1,b2,e2,status);
                }

                int do_compare(level_type level,
                               CharType const *b1,CharType const *e1,
                               CharType const *b2,CharType const *e2) const BOOST_OVERRIDE
                {
                    UErrorCode status=U_ZERO_ERROR;

                    int res = do_real_compare(level,b1,e1,b2,e2,status);

                    if(U_FAILURE(status))
                            throw std::runtime_error(std::string("Collation failed:") + u_errorName(status));
                    if(res < 0)
                        return -1;
                    else if(res > 0)
                        return 1;
                    return 0;
                }

                std::vector<uint8_t> do_basic_transform(level_type level,CharType const *b,CharType const *e) const
                {
                    icu::UnicodeString str=cvt_.icu(b,e);
                    std::vector<uint8_t> tmp;
                    tmp.resize(str.length() + 1u);
                    icu::Collator *collate = get_collator(level);
                    const int len = collate->getSortKey(str,&tmp[0],tmp.size());
                    if(len > int(tmp.size())) {
                        tmp.resize(len);
                        collate->getSortKey(str,&tmp[0],tmp.size());
                    }
                    else
                        tmp.resize(len);
                    return tmp;
                }
                std::basic_string<CharType> do_transform(level_type level,CharType const *b,CharType const *e) const BOOST_OVERRIDE
                {
                    std::vector<uint8_t> tmp = do_basic_transform(level,b,e);
                    return std::basic_string<CharType>(tmp.begin(),tmp.end());
                }

                long do_hash(level_type level,CharType const *b,CharType const *e) const BOOST_OVERRIDE
                {
                    std::vector<uint8_t> tmp = do_basic_transform(level,b,e);
                    tmp.push_back(0);
                    return gnu_gettext::pj_winberger_hash_function(reinterpret_cast<char *>(&tmp.front()));
                }

                collate_impl(cdata const &d) :
                    cvt_(d.encoding),
                    locale_(d.locale),
                    is_utf8_(d.utf8)
                {

                }
                icu::Collator *get_collator(level_type ilevel) const
                {
                    int l = limit(ilevel);
                    static const icu::Collator::ECollationStrength levels[level_count] =
                    {
                        icu::Collator::PRIMARY,
                        icu::Collator::SECONDARY,
                        icu::Collator::TERTIARY,
                        icu::Collator::QUATERNARY,
                        icu::Collator::IDENTICAL
                    };

                    icu::Collator *col = collates_[l].get();
                    if(col)
                        return col;

                    UErrorCode status=U_ZERO_ERROR;

                    collates_[l].reset(icu::Collator::createInstance(locale_,status));

                    if(U_FAILURE(status))
                        throw std::runtime_error(std::string("Creation of collate failed:") + u_errorName(status));

                    collates_[l]->setStrength(levels[l]);
                    return collates_[l].get();
                }

            private:
                static const int level_count = 5;
                icu_std_converter<CharType>  cvt_;
                icu::Locale locale_;
                mutable boost::thread_specific_ptr<icu::Collator> collates_[level_count];
                bool is_utf8_;
            };


            #if BOOST_LOCALE_WITH_STRINGPIECE
            template<>
            int collate_impl<char>::do_real_compare(
                                    level_type level,
                                    char const *b1,char const *e1,
                                    char const *b2,char const *e2,
                                    UErrorCode &status) const
            {
                if(is_utf8_)
                    return do_utf8_compare(level,b1,e1,b2,e2,status);
                else
                    return do_ustring_compare(level,b1,e1,b2,e2,status);
            }
            #endif

            std::locale create_collate(std::locale const &in,cdata const &cd,character_facet_type type)
            {
                switch(type) {
                case char_facet:
                    return std::locale(in,new collate_impl<char>(cd));
                case wchar_t_facet:
                    return std::locale(in,new collate_impl<wchar_t>(cd));
                #ifdef BOOST_LOCALE_ENABLE_CHAR16_T
                case char16_t_facet:
                    return std::locale(in,new collate_impl<char16_t>(cd));
                #endif
                #ifdef BOOST_LOCALE_ENABLE_CHAR32_T
                case char32_t_facet:
                    return std::locale(in,new collate_impl<char32_t>(cd));
                #endif
                default:
                    return in;
                }
            }

        } /// impl_icu

    } // locale
} // boost

