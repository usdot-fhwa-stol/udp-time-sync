#!/bin/bash

set -ex
# export BUILD_ARCHITECTURE=arm64
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
# Install rapidjson commit instead of release since there has been no new release since August 2016
# but repo is still under active development
echo " ------> Install rapidjson..."
cd /tmp
git clone https://github.com/Tencent/rapidjson
cd rapidjson/
git checkout a95e013b97ca6523f32da23f5095fcc9dd6067e5
cmake -Bbuild -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DRAPIDJSON_BUILD_DOC=OFF -DRAPIDJSON_BUILD_EXAMPLES=OFF -DRAPIDJSON_BUILD_TESTS=OFF
cmake --build build
cmake --install build
cd .. 
rm -r rapidjson



/opt/carma/scripts/install_dependencies_script.sh python3-dev carma-clock-1 udp-socket-1

ldconfig
