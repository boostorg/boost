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

report_info()
{
cat - > comment.html <<HTML
<table style="border-spacing: 0.5em;">
    <tr>
        <td style="vertical-align: top;"><tt>uname</tt></td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`uname -a`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;"><tt>uptime</tt></td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`uptime`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;"><tt>vmstat</tt></td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`vmstat`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;"><tt>xsltproc</tt></td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`xsltproc --version`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;"><tt>python</tt></td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`python --version 2>&1`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;">previous run</td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`cat previous.txt`
            </pre>
        </td>
    </tr>
    <tr>
        <td style="vertical-align: top;">current run</td>
        <td>
            <pre style="border: 1px solid #666; overflow: auto;">
`date -u`
            </pre>
        </td>
    </tr>
</table>
HTML
    date -u > previous.txt
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
    report_info
    python "${boost}/tools/regression/xsl_reports/boost_wide_report.py" \
        --locate-root="${root}" \
        --tag=${tag} \
        --expected-results="${boost}/status/expected_results.xml" \
        --failures-markup="${boost}/status/explicit-failures-markup.xml" \
        --comment="comment.html" \
        --user="" \
        --reports="i,dd,ds,n"
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
    scp ${1}.zip.bz2 grafik@beta.boost.org:/home/grafik/www.boost.org/testing/incoming/
    ssh grafik@beta.boost.org bunzip2 /home/grafik/www.boost.org/testing/incoming/${1}.zip.bz2
}

build_all ${1}
