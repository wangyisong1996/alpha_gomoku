#!/usr/bin/env sh

# ~/caffe/build/tools/caffe train --solver=train.prototxt --weights=60000.caffemodel  2> train.log
~/caffe/build/tools/caffe train --solver=train.prototxt 2> train.log
