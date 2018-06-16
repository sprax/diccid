Docker Image Circle CI for Development in C++.
==============================================

CircleCI [![CircleCI](https://circleci.com/gh/sprax/diccid.svg?style=svg)](https://circleci.com/gh/sprax/diccid)

# Steps
1. Clone public repository into external directory.
  - Build locally, use ADD or COPY commands to get products and sources (at least the headers) into Docker image
  - If some products change often, consider checking them into repository, so they get checked out on top of Docker image. 
