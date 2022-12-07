
// Copyright 2018 Peter Dimov.
// Copyrigth 2022 Jeff Trull.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

// Most test coverage of Wave happens with the "eat_whitespace" hooks in effect
// This adds some coverage for Wave with the (minimal) default hooks

#include <boost/wave.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

int main()
{
    std::string input(
        // check LINE directives after conditional (false) sections 
        "#if 0\n"
        "int aa;\n"
        "#endif\n"
        "char c1;\n"     // #LINE 4

        "#ifdef FOO\n"
        "int bb;\n"
        "#endif\n"
        "char c2;\n"     // #LINE 8

        "#ifndef __FILE__\n"
        "int cc;\n"
        "#endif\n"
        "char c3;\n"     // #LINE 12
    );

    using namespace boost::wave;

    typedef std::pair<token_id, char const*> tokdata_type;
    typedef std::vector<tokdata_type> tokdata_list_type;
    tokdata_list_type expected{
        {T_PP_LINE, "#line"},
        {T_SPACE, " "},
        {T_INTLIT, "4"},
        {T_SPACE, " "},
        {T_STRINGLIT, nullptr},  // exact filepath is not interesting
        {T_NEWLINE, "\n"},
        {T_CHAR, "char"},
        {T_SPACE, " "},
        {T_IDENTIFIER, "c1"},
        {T_SEMICOLON, ";"},
        {T_NEWLINE, "\n"},

        {T_PP_LINE, "#line"},
        {T_SPACE, " "},
        {T_INTLIT, "8"},
        {T_SPACE, " "},
        {T_STRINGLIT, nullptr},
        {T_NEWLINE, "\n"},
        {T_CHAR, "char"},
        {T_SPACE, " "},
        {T_IDENTIFIER, "c2"},
        {T_SEMICOLON, ";"},
        {T_NEWLINE, "\n"},

        {T_PP_LINE, "#line"},
        {T_SPACE, " "},
        {T_INTLIT, "12"},
        {T_SPACE, " "},
        {T_STRINGLIT, nullptr},
        {T_NEWLINE, "\n"},
        {T_CHAR, "char"},
        {T_SPACE, " "},
        {T_IDENTIFIER, "c3"},
        {T_SEMICOLON, ";"},
        {T_NEWLINE, "\n"},

        {T_EOF, nullptr}
};

    try
    {
        typedef cpplexer::lex_token<> token_type;
        typedef cpplexer::lex_iterator<token_type> lex_iterator_type;
        typedef context<std::string::iterator, lex_iterator_type> context_type;

        context_type ctx( input.begin(), input.end(), "input.cpp" );

        tokdata_list_type::const_iterator first_expect = expected.begin();
        tokdata_list_type::const_iterator last_expect = expected.end();
        for( context_type::iterator_type first = ctx.begin(), last = ctx.end(); first != last; ++first, ++first_expect )
        {
            if (first_expect == last_expect)
            {
                std::cerr << "more tokens were produced than expected\n";
                return 3;
            }

            if (token_id(*first) != first_expect->first)
            {
                std::cerr << "expected " << get_token_name(first_expect->first) << ", got " << get_token_name(token_id(*first)) << "\n";
                return 3;
            }

            if ((first_expect->second != nullptr) && (first_expect->second != first->get_value()))
            {
                std::cerr << "expected token value " << first_expect->second << ", got " << first->get_value() << "\n";
                return 3;
            }
                
            std::cout << first->get_value();
        }

        if (first_expect != last_expect)
        {
            std::cerr << "fewer tokens were produced than expected\n";
            return 3;
        }

        return 0;
    }
    catch( cpp_exception const & x )
    {
        std::cerr << x.file_name() << "(" << x.line_no() << "): " << x.description() << std::endl;
        return 1;
    }
    catch( std::exception const & x )
    {
        std::cerr << "Exception: " << x.what() << std::endl;
        return 2;
    }
}
