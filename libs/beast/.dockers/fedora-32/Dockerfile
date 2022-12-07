FROM fedora:32

RUN dnf update -y
RUN dnf -y groupinstall "Development Tools"
RUN dnf install -y cmake openssl-devel clang libcxx-devel libcxxabi-devel bzip2-devel which zlib-devel

WORKDIR /devel
RUN git clone -b develop https://github.com/boostorg/boost.git
COPY .dockers/fedora-32/user-config.jam /devel/boost/
COPY .dockers/fedora-32/tests.sh /devel/boost/

WORKDIR /devel/boost
RUN git submodule update --init --recursive
COPY . /devel/boost/libs/beast/

#RUN cd libs/beast && git checkout develop && git pull origin develop
RUN ./bootstrap.sh
RUN ./b2 toolset=gcc variant=release cxxstd=latest headers
RUN ./tests.sh || true
