# Copyright 2022 Nikita Kniazev
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from MockProgram import *

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('cl', '/Zm800', '-nologo', input_file(source='lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend')
    command('cl', '/Zm800', '-nologo', input_file(source='main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend')

if allow_properties("variant=release", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('cl', '/Zm800', '-nologo', input_file(source='lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\release\threading-multi\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/O2', '/Ob2', '/W3', '/MD', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/Gw', '/favor:blend', '-DNDEBUG')
    command('cl', '/Zm800', '-nologo', input_file(source='main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\release\threading-multi\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/O2', '/Ob2', '/W3', '/MD', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/Gw', '/favor:blend', '-DNDEBUG')

if allow_properties("variant=debug", "link=static", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('cl', '/Zm800', '-nologo', input_file(source='lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\link-static\threading-multi\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend')
    command('cl', '/Zm800', '-nologo', input_file(source='main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\link-static\threading-multi\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend')

if allow_properties("variant=debug", "link=static", "threading=single", "runtime-link=static", "windows-api=desktop"):
    command('cl', '/Zm800', '-nologo', input_file(source='lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '/MTd')
    command('cl', '/Zm800', '-nologo', input_file(source='main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '/MTd')

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=store"):
    command('cl', '/Zm800', '/ZW', '/EHsc', '-nologo', input_file(source=r'lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '-DWINAPI_FAMILY=WINAPI_FAMILY_APP', '-D_WIN32_WINNT=0x0602')
    command('cl', '/Zm800', '/ZW', '/EHsc', '-nologo', input_file(source=r'main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '-DWINAPI_FAMILY=WINAPI_FAMILY_APP', '-D_WIN32_WINNT=0x0602')

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=phone"):
    command('cl', '/Zm800', '/ZW', '/EHsc', '-nologo', input_file(source=r'lib.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\lib.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '-DWINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP', '-D_WIN32_WINNT=0x0602')
    command('cl', '/Zm800', '/ZW', '/EHsc', '-nologo', input_file(source=r'main.cpp'), '-c', arg('-Fo', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\main.obj')), '-TP', '/wd4675', '/EHs', '/GR', '/Zc:throwingNew', '/Z7', '/Od', '/Ob0', '/W3', '/MDd', '/Zc:forScope', '/Zc:wchar_t', '/Zc:inline', '/favor:blend', '-DWINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP', '-D_WIN32_WINNT=0x0602')

main()
