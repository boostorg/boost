#!/bin/sh

#~ Copyright Redshift Software, Inc. 2007
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

export PATH=/bin:/usr/bin:${PATH}

timestamp=`date +%F-%H-%M-%S-%Z`
branch=$1
tag=boost-${1/\/*}-${timestamp}
export_dir=boost-$$

rm_c()
{
  while read f; do
    rm -f ${f}
  done
}
svn_export()
{
  svn export file:///home/subversion/boost/${branch} ${tag} \
    | cut --bytes=6- \
    | star -c -D -to-stdout -d artype=tar list=- 2>/dev/null \
    | bzip2 -3 -c \
    | tee $1 \
    | tar -jtf - \
    | rm_c
}

cd /tmp
rm -rf ${export_dir}
mkdir ${export_dir}
cd ${export_dir}
mkfifo out.tbz2
svn_export out.tbz2 &
#cat out.tbz2 > ../${tag}.tar.bz2
cat out.tbz2
cd /tmp
rm -rf ${export_dir}

