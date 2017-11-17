#!/usr/bin/env sh

CaffeRoot=/home/yangliwei/caffe/caffe-master
ProjectDir=myProjects/ageAndGender
DataDir=${CaffeRoot}/${ProjectDir}/data

meanTool=${CaffeRoot}/build/tools/compute_image_mean
trainLmdb=${DataDir}/image_train_lmdb
meanFileName=gender_mean.binaryproto

rm -f ${DataDir}/${meanFileName}


echo "Start to generate mean binaryproto..."
${meanTool} ${trainLmdb} ${DataDir}/${meanFileName}
echo "Done."
