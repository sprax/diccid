#!/usr/bin/sh 
# @file: build.sh
set -ex
# SET THE FOLLOWING VARIABLES
# docker hub username
USERNAME=sprax
# image name
IMAGE=diccid
docker build -t $USERNAME/$IMAGE:latest .

