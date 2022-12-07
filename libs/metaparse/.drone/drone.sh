#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

echo "using clang : 3.5 : clang++ ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "4e3fa5de4a-f44b5bccc0" ]; then

echo '==================================> INSTALL'

sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "fea35f4805-f44b5bccc0" ]; then

echo '==================================> INSTALL'

echo "using clang : 3.6 : clang++ ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "dfd0ac5626-f44b5bccc0" ]; then

echo '==================================> INSTALL'

echo "using gcc : 4.6 : g++-4.6 ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "0e39902f2e-f44b5bccc0" ]; then

echo '==================================> INSTALL'

echo "using gcc : 4.7 : g++-4.7 ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "91b94bb329-f44b5bccc0" ]; then

echo '==================================> INSTALL'

echo "using gcc : 4.8 : g++-4.8 ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

elif [ "$DRONE_JOB_BUILDTYPE" == "bb699d113a-f44b5bccc0" ]; then

echo '==================================> INSTALL'

echo "using gcc : 4.9 : g++-4.9 ;" > ~/user-config.jam
sudo pip install pep8 pylint

echo '==================================> SCRIPT'

if which pep8; then pep8 tools/string_headers.py; fi
if which pylint; then pylint tools/string_headers.py; fi
if [ "TOOLSET" == "clang" ]; then tools/get_clang.sh ${CXX_VER}; fi
export PATH="${PWD}:${PATH}"
if [ "${STD}" ]; then CXXFLAGS="cxxflags=\"-std=${STD}\""; fi
echo "using doxygen ;" >> ~/user-config.jam
echo "using xsltproc ;" >> ~/user-config.jam
echo "using quickbook ;" >> ~/user-config.jam
tools/build_environment.py --dep_json tools/deps.json --action checkout --out .boost
cd .boost
cd libs
mkdir metaparse
mv ../../* ../../.git ../../.travis.yml metaparse
cd ..
cd tools/boostbook
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xml-4.2.zip
wget http://abel.web.elte.hu/boost_book_dependencies/docbook-xsl-1.75.2.tar.gz
wget http://abel.web.elte.hu/boost_book_dependencies/fop-0.94-bin-jdk1.4.tar.gz
./setup_boostbook.sh
cd ../..
cat ~/user-config.jam
./bootstrap.sh --with-toolset=$TOOLSET
./b2 headers
cd libs
cd test/build
../../../b2 -d+2
cd ../..
cd metaparse
echo "Checking if all automatically generated files are up to date"
rm doc/getting_started_*.qbk doc/before_*.qbk example/getting_started/*.hpp
tools/generate_all.py
git add -N .
git diff --exit-code
echo "Building the unit tests and the examples"
cd test
../../../b2 -d+2 ${CXXFLAGS}
sed -i 's/#include <boost\/metaparse\/[^/]*\.hpp>/#include <boost\/metaparse.hpp>/' *.?pp
../../../b2 -d+2 ${CXXFLAGS}
cd ..
cd example
../../../b2 -d+2 ${CXXFLAGS}
cd ..
echo "Generating the documentation"
cd doc
../../../b2 boostdoc

fi
