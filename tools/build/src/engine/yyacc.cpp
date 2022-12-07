/* Copyright 2002, 2020 Rene Rivera.
** Distributed under the Boost Software License, Version 1.0.
** (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#include <cstdio>
#include <string>
#include <algorithm>
#include <cctype>
#include <set>
#include <cstring>

/*
# yyacc - yacc wrapper
#
# Allows tokens to be written as `literal` and then automatically
# substituted with #defined tokens.
#
# Usage:
#    yyacc file.y filetab.h file.yy
#
# inputs:
#    file.yy        yacc grammar with ` literals
#
# outputs:
#    file.y        yacc grammar
#    filetab.h    array of string <-> token mappings
#
# 3-13-93
#    Documented and p moved in sed command (for some reason,
#    s/x/y/p doesn't work).
# 10-12-93
#    Take basename as second argument.
# 12-31-96
#    reversed order of args to be compatible with GenFile rule
# 11-20-2002
#    Reimplemented as a C program for portability. (Rene Rivera)
# 05-xx-2020
#    Reimplement yet again, in C++. (Rene Rivera)
*/

static const std::string usage[] = {
    "yyacc <grammar output.y> <token table output.h> <grammar source.yy>"
    };

void print_usage()
{
    for (auto u: usage)
    {
        std::printf("%s\n", u.c_str());
    }
}

std::string tokenize_string(std::string s)
{
    std::string result = s;
    if (s == ":") result = "_colon";
    else if (s == "!") result = "_bang";
    else if (s == "!=") result = "_bang_equals";
    else if (s == "&&") result = "_amperamper";
    else if (s == "&") result = "_amper";
    else if (s == "+") result = "_plus";
    else if (s == "+=") result = "_plus_equals";
    else if (s == "||") result = "_barbar";
    else if (s == "|") result = "_bar";
    else if (s == ";") result = "_semic";
    else if (s == "-") result = "_minus";
    else if (s == "<") result = "_langle";
    else if (s == "<=") result = "_langle_equals";
    else if (s == ">") result = "_rangle";
    else if (s == ">=") result = "_rangle_equals";
    else if (s == ".") result = "_period";
    else if (s == "?") result = "_question";
    else if (s == "?=") result = "_question_equals";
    else if (s == "=") result = "_equals";
    else if (s == ",") result = "_comma";
    else if (s == "[") result = "_lbracket";
    else if (s == "]") result = "_rbracket";
    else if (s == "{") result = "_lbrace";
    else if (s == "}") result = "_rbrace";
    else if (s == "(") result = "_lparen";
    else if (s == ")") result = "_rparen";

    std::transform(
        result.begin(), result.end(), result.begin(),
        [](unsigned char c){ return std::toupper(c); });
    return result+"_t";
}

struct literal
{
    std::string string;
    std::string token;

    bool operator<(const literal & x) const
    {
        return this->string < x.string;
    }
};

int main(int argc, char ** argv)
{
    int result = 0;
    if (argc != 4)
    {
        print_usage();
        result = 1;
    }
    else
    {
        FILE * token_output_f = 0;
        FILE * grammar_output_f = 0;
        FILE * grammar_source_f = 0;

        grammar_source_f = fopen(argv[3],"r");
        if (grammar_source_f == 0) { result = 1; }
        if (result == 0)
        {
            std::set<literal> literals;
            char l[2048];
            while (1)
            {
                if (fgets(l,2048,grammar_source_f) != 0)
                {
                    char * c = l;
                    while (1)
                    {
                        char * c1 = std::strchr(c,'`');
                        if (c1 != 0)
                        {
                            char * c2 = std::strchr(c1+1,'`');
                            if (c2 != 0)
                            {
                                auto l = std::string(c1+1,c2-c1-1);
                                literals.insert({ l, tokenize_string(l) });
                                c = c2+1;
                            }
                            else
                                break;
                        }
                        else
                            break;
                    }
                }
                else
                {
                    break;
                }
            }
            token_output_f = std::fopen(argv[2],"w");
            if (token_output_f != 0)
            {
                for (const literal & l: literals)
                {
                    std::fprintf(token_output_f,"    { \"%s\", %s },\n",l.string.c_str(), l.token.c_str());
                }
                std::fclose(token_output_f);
            }
            else
                result = 1;
            if (result == 0)
            {
                grammar_output_f = std::fopen(argv[1],"w");
                if (grammar_output_f != 0)
                {
                    for (const literal & l: literals)
                    {
                        fprintf(grammar_output_f,"%%token %s\n",l.token.c_str());
                    }
                    rewind(grammar_source_f);
                    while (1)
                    {
                        if (fgets(l,2048,grammar_source_f) != 0)
                        {
                            char * c = l;
                            while (1)
                            {
                                char * c1 = strchr(c,'`');
                                if (c1 != 0)
                                {
                                    char * c2 = strchr(c1+1,'`');
                                    if (c2 != 0)
                                    {
                                        auto replacement = literals.find({std::string(c1+1,c2-c1-1), ""});
                                        *c1 = 0;
                                        std::fprintf(grammar_output_f,"%s%s",c,replacement->token.c_str());
                                        c = c2+1;
                                    }
                                    else
                                    {
                                        std::fprintf(grammar_output_f,"%s",c);
                                        break;
                                    }
                                }
                                else
                                {
                                    std::fprintf(grammar_output_f,"%s",c);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    std::fclose(grammar_output_f);
                }
                else
                    result = 1;
            }
        }
        if (result != 0)
        {
            perror("yyacc");
        }
    }
    return result;
}
