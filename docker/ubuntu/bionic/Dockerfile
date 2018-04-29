# Docker build environment for Boost on Ubuntu Bionic
#
# Copyright (c) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

FROM ubuntu:bionic
MAINTAINER James E. King III <jking@apache.org>
ENV CONTAINER_USER=boost
ENV DEBIAN_FRONTEND noninteractive
WORKDIR /boost

# Load apt-utils first, fixes warnings

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      apt-utils

# Basic utilities

RUN apt-get install -y --no-install-recommends \
      bash-completion \
      bzip2 \
      ca-certificates \
      curl \
      file \
      git \
      patch \
      procps \
      sudo \
      unzip \
      vim \
      wget

# C++

RUN apt-get install -y --no-install-recommends \
      build-essential \
      clang++-6 \
      cmake \
      gdb \
      lcov \
      valgrind

#################################################################
## Boost Library Dependencies
## Do not "de-dupe" - this is a canonical list of dependencies!
#################################################################

# asio
RUN apt-get install -y --no-install-recommends \
      libssl-dev

# beast
RUN apt-get install -y --no-install-recommends \
      libssl-dev \
      zlib1g-dev

# gil
RUN apt-get install -y --no-install-recommends \
      libjpeg-dev \
      libpng-dev \
      libtiff-dev \
      zlib1g-dev

# integer
RUN apt-get install -y --no-install-recommends \
      libgmp-dev

# interval (for one disabled example)
RUN apt-get install -y --no-install-recommends \
      libgmp-dev \
      libmpfr-dev

# iostreams
RUN apt-get install -y --no-install-recommends \
      libbz2-dev \
      liblzma-dev \
      libzstd-dev \
      zlib1g-dev

# locale
RUN apt-get install -y --no-install-recommends \
      libicu-dev

# mpi
RUN apt-get install -y --no-install-recommends \
      libopenmpi-dev

# python
RUN apt-get install -y --no-install-recommends \
      python3-dev

# sync
#     pthread-win32, but not on Unix

# thread
#     pthread-win32, but not on Unix

#################################################################
# Prerequisites for boostbook (does what setup_boostbook.sh does)
# for documentation builds to work

RUN apt-get install -y --no-install-recommends \
      xsltproc \
      doxygen \
      openjdk-11-jre-headless

RUN cd /opt && \
    wget --quiet http://sourceforge.net/projects/docbook/files/docbook-xsl/1.75.2/docbook-xsl-1.75.2.tar.gz && \
    tar xzf docbook-xsl-1.75.2.tar.gz && \
    rm docbook-xsl-1.75.2.tar.gz

RUN cd /opt && \
    mkdir docbook-dtd-4.2 && \
    cd docbook-dtd-4.2 && \
    wget --quiet http://www.oasis-open.org/docbook/xml/4.2/docbook-xml-4.2.zip && \
    unzip -q docbook-xml-4.2.zip && \
    rm docbook-xml-4.2.zip

RUN cd /opt && \
    wget --quiet http://archive.apache.org/dist/xmlgraphics/fop/binaries/fop-0.94-bin-jdk1.4.tar.gz && \
    tar xzf fop-0.94-bin-jdk1.4.tar.gz && \
    rm fop-0.94-bin-jdk1.4.tar.gz

#################################################################
# Build as a regular user
# Credit: https://github.com/delcypher/docker-ubuntu-cxx-dev/blob/master/Dockerfile
# License: None specified at time of import
# Add non-root user for container but give it sudo access.
# Password is the same as the username
RUN useradd -m ${CONTAINER_USER} && \
    echo ${CONTAINER_USER}:${CONTAINER_USER} | chpasswd && \
    echo "${CONTAINER_USER}  ALL=(root) ALL" >> /etc/sudoers
RUN chsh --shell /bin/bash ${CONTAINER_USER}
USER ${CONTAINER_USER}
#################################################################

ADD --chown=boost user-config.jam /home/boost/user-config.jam
