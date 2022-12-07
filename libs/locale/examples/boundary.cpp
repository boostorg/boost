//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/locale.hpp>
#include <iostream>
#include <ctime>

int main()
{
    using namespace boost::locale;

    generator gen;
    // Make system default locale global
    std::locale loc = gen("");
    // We need the boundary facet, currently only available via ICU
    if(!std::has_facet<boundary::boundary_indexing<char>>(loc))
    {
        std::cout << "boundary detection not implemented in this environment\n";
        return 0;
    }
    std::locale::global(loc);
    std::cout.imbue(loc);


    std::string text="Hello World! あにま! Linux2.6 and Windows7 is word and number. שָלוֹם עוֹלָם!";

    std::cout << text << std::endl;

    boundary::ssegment_index index(boundary::word,text.begin(),text.end());
    boundary::ssegment_index::iterator p,e;

    for(p=index.begin(),e=index.end();p!=e;++p) {
        std::cout << "Part [" << *p << "] has ";
        if(p->rule() & boundary::word_number)
            std::cout << "number(s) ";
        if(p->rule() & boundary::word_letter)
            std::cout << "letter(s) ";
        if(p->rule() & boundary::word_kana)
            std::cout << "kana character(s) ";
        if(p->rule() & boundary::word_ideo)
            std::cout << "ideographic character(s) ";
        if(p->rule() & boundary::word_none)
            std::cout << "no word characters";
        std::cout << std::endl;
    }

    index.map(boundary::character,text.begin(),text.end());

    for(p=index.begin(),e=index.end();p!=e;++p) {
        std::cout << "|" << *p ;
    }
    std::cout << "|\n\n";

    index.map(boundary::line,text.begin(),text.end());

    for(p=index.begin(),e=index.end();p!=e;++p) {
        std::cout << "|" << *p ;
    }
    std::cout << "|\n\n";

    index.map(boundary::sentence,text.begin(),text.end());

    for(p=index.begin(),e=index.end();p!=e;++p) {
        std::cout << "|" << *p ;
    }
    std::cout << "|\n\n";

}

// boostinspect:noascii
