FROM centos:7

RUN yum update -y
RUN yum install -y epel-release
RUN yum -y groupinstall "Development Tools"
RUN yum install -y cmake openssl-devel clang libcxx-devel libcxxabi-devel bzip2-devel which zlib-devel

WORKDIR /devel
RUN git clone -b develop https://github.com/boostorg/boost.git
COPY .dockers/centos-7/user-config.jam /devel/boost/
COPY .dockers/centos-7/tests.sh /devel/boost/

WORKDIR /devel/boost
RUN git submodule update --init --recursive
COPY . /devel/boost/libs/beast/

RUN ./bootstrap.sh
RUN ./b2 toolset=gcc variant=release cxxstd=latest headers
RUN ./tests.sh || true
