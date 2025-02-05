#!/bin/bash

set -e

${CARMA_OPT_DIR}/scripts/build_script.sh -p $@ -DBUILD_TESTS=ON -DCREATE_DEB_PACKAGE=ON
