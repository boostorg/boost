#!/bin/bash
#no hash verification
sudo apt update && apt full-upgrade -y && sudo apt install g++-14 curl grep gzip tar wget

wget https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
gunzip boost_1_87_0.tar.gz && tar -xf boost_1_87_0.tar
sudo cp boost_1_87_0/boost /usr/include/ -r
rm boost_1_87_0 boost_1_87_0.tar -r -f

