#!/usr/bin/python

# Copyright 2021 Dmitry Arkhipov (grisumbras@gmail.com)
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test conditionals returning conditionals

import BoostBuild

t = BoostBuild.Tester()

t.write("a.cpp", "")

t.write("jamroot.jam", """
import feature ;
import common ;

feature.feature the_feature : false true : propagated ;

rule add-feature ( properties * )
{
    return <the_feature>true ;
}

rule maker ( targets * : sources * : properties * )
{
    if ! <the_feature>true in $(properties)
    {
        EXIT "Need <the_feature>true to build" ;
    }
    CMD on $(targets) = [ common.file-creation-command ] ;
}

actions maker
{
    $(CMD) $(<) ;
}

make a : a.cpp : maker : <variant>debug:<conditional>@add-feature ;
""")

t.run_build_system()

t.cleanup()
