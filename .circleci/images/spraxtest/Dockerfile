# sprax's public Docker file for testing stuff built on C++
# NOTE: build-essential contains: dpkg-dev, g++, gcc, libc6-dev, and make.
# Pushed to Docker Hub as sprax/diccid-kid-a:0.0.2
FROM ubuntu:16.04
RUN apt-get update -qq \
  && apt-get install -qy --no-install-recommends \
    build-essential \
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
    openssh-client \
    protobuf-compiler \
    wget \
  && rm -rf /var/lib/apt/lists/*
RUN wget -O drake.tar.gz https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-xenial.tar.gz
RUN tar -xf drake.tar.gz -C /opt
