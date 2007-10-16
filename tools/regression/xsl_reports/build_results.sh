#!/bin/sh

#~ Copyright Redshift Software, Inc. 2007
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

set -e

build_all()
{
    update_tools ${1}
    build_results ${1}
    upload_results ${1}
}

update_tools()
{
    cwd=`pwd`
    cd boost
    svn up
    cd "${cwd}"
}

build_results()
{
    cwd=`pwd`
    cd ${1}
    root=`pwd`
    boost=${cwd}/boost
    case ${1} in
        trunk) tag=trunk ;;
        release) tag=branches/release ;;
    esac
    python "${boost}/tools/regression/xsl_reports/boost_wide_report.py" \
        --locate-root="${root}" \
        --tag=${tag} \
        --expected-results="${boost}/status/expected_results.xml" \
        --failures-markup="${boost}/status/explicit-failures-markup.xml" \
        --comment="" \
        --user=""
    cd "${cwd}"
}

upload_results()
{
    cwd=`pwd`
    cd ${1}/all
    rm -f ../../${1}.zip*
    zip -r -9 ../../${1} * -x '*.xml'
    cd "${cwd}"
    bzip2 -9 ${1}.zip
    scp ${1}.zip.bz2 beta.boost.org:/home/grafik/www.boost.org/testing/incoming/
    ssh beta.boost.org bunzip2 /home/grafik/www.boost.org/testing/incoming/${1}.zip.bz2
}

build_all ${1}
