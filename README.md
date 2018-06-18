Docker Image Circle CI for Development in C++.
==============================================

CircleCI [![CircleCI](https://circleci.com/gh/sprax/diccid.svg?style=svg)](https://circleci.com/gh/sprax/diccid)

## Steps
1. Clone public or private git, svn, or other repository into external directory.
  - Build locally, use ADD or COPY commands to get products and sources (at least the headers) into Docker image
  - If some products change often, consider checking them into repository, so they get checked out on top of Docker image. 
2. If you must change the cloned repo and don't have access rights to push up a branch, fork it and go back to 1.
3. Clone, check out, merge or rebase inside the docker only if you cannot build the repo outside the docker.
4. Prefer a chain of docker images over (3) -- copy, build, push in Docker A, retrieve outside docker, copy into Docker B.
