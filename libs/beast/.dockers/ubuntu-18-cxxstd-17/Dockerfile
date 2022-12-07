FROM ubuntu:18.04

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y git cmake libssl-dev clang libc++-10-dev libc++abi-10-dev libbz2-dev zlib1g-dev

WORKDIR /devel
RUN git clone -b develop https://github.com/boostorg/boost.git
COPY .dockers/ubuntu-18-cxxstd-17/user-config.jam /devel/boost/
COPY .dockers/ubuntu-18-cxxstd-17/tests.sh /devel/boost/ 

WORKDIR /devel/boost
RUN git submodule update --init --recursive
COPY . /devel/boost/libs/beast/

RUN ./bootstrap.sh
RUN ./b2 toolset=gcc variant=release cxxstd=latest headers
RUN ./tests.sh || true
