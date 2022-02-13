#!/bin/bash

set -x

pushd build/Linux/MinSizeRel/
#pushd build/Linux/Debug/
rsync -r -l . ~/workspace/dev/gme-build
popd

