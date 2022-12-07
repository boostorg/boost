# Copyright 2022 Nikita Kniazev
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from MockProgram import *

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\lib.obj'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\l1.dll')), '/DLL', arg('/IMPLIB:', output_file(r'bin\msvc-14.3\debug\threading-multi\l1.implib')))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\main.obj'), input_file(r'bin\msvc-14.3\debug\threading-multi\l1.implib'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\test.exe')))

if allow_properties("variant=release", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\release\threading-multi\lib.obj'), '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\release\threading-multi\l1.dll')), '/DLL', arg('/IMPLIB:', output_file(r'bin\msvc-14.3\release\threading-multi\l1.implib')))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\release\threading-multi\main.obj'), input_file(r'bin\msvc-14.3\release\threading-multi\l1.implib'), '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\release\threading-multi\test.exe')))

if allow_properties("variant=debug", "link=static", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('link', '/lib', '/NOLOGO', arg('/out:', output_file(r'bin\msvc-14.3\debug\link-static\threading-multi\l1.lib')), input_file(r'bin\msvc-14.3\debug\link-static\threading-multi\lib.obj'))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\link-static\threading-multi\main.obj'), input_file(r'bin\msvc-14.3\debug\link-static\threading-multi\l1.lib'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\link-static\threading-multi\test.exe')))

if allow_properties("variant=debug", "link=static", "threading=single", "runtime-link=static", "windows-api=desktop"):
    command('link', '/lib', '/NOLOGO', arg('/out:', output_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\l1.lib')), input_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\lib.obj'))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\main.obj'), input_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\l1.lib'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\link-static\runtime-link-static\test.exe')))

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=store"):
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\lib.obj'), '/DEBUG', '/APPCONTAINER', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\l1.dll')), '/DLL', arg('/IMPLIB:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\l1.implib')))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\main.obj'), input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\l1.implib'), '/DEBUG', '/APPCONTAINER', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-store\test.exe')))

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=phone"):
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\lib.obj'), '/DEBUG', '/APPCONTAINER', '/NODEFAULTLIB:kernel32.lib', '/NODEFAULTLIB:ole32.lib', 'WindowsPhoneCore.lib', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\l1.dll')), '/DLL', arg('/IMPLIB:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\l1.implib')))
    command('link', '/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\main.obj'), input_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\l1.implib'), '/DEBUG', '/APPCONTAINER', '/NODEFAULTLIB:kernel32.lib', '/NODEFAULTLIB:ole32.lib', 'WindowsPhoneCore.lib', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\windows-api-phone\test.exe')))

main()
