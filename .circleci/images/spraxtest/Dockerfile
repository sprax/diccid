# sprax's public Docker file for testing stuff built on C++
FROM ubuntu:16.04
RUN apt-get update -qq \
  && apt-get install -qy --no-install-recommends \
    ca-certificates \
    cmake \
    coinor-libclp-dev \
    coinor-libipopt-dev \
    freeglut3-dev \
    git \
    libeigen3-dev \
    libpcl-dev \
    libgtest-dev \
    libboost-all-dev \
    libeigen3-dev \
    libgflags-dev \
    libgtest-dev \
    libjpeg8-dev \
    libnlopt-dev \
    libpng12-dev \
    libprotobuf-dev \
    libtiff5-dev \
    libtinyxml-dev \
    libtinyxml2-dev \
    libxml2-dev \
    libxt-dev \
    libyaml-cpp-dev \
    make \
    openssh-client \
    protobuf-compiler \
    wget \
  && rm -rf /var/lib/apt/lists/*
RUN wget -O drake.tar.gz https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-xenial.tar.gz
RUN tar -xf drake.tar.gz -C /opt

