#!/usr/bin/env sh

CaffeRoot=/home/yangliwei/caffe/caffe-master
ProjectDir=myProjects/ageAndGender
DataDir=${CaffeRoot}/${ProjectDir}/data

TrainDataList=train.txt
TestDataList=test.txt

trainLmdbName=image_train_lmdb
testLmdbName=image_test_lmdb
trainLmdb=${DataDir}/${trainLmdbName}
testLmdb=${DataDir}/${testLmdbName}

echo "Create train lmdb.."
rm -rf ${trainLmdb}
convert_imageset --shuffle --resize_height=256 --resize_width=256 ${DataDir} ${TrainDataList} ${trainLmdb}

echo "Create test lmdb.."
rm -rf ${testLmdb}
convert_imageset -shuffle --resize_width=256 --resize_height=256 ${DataDir} ${TestDataList} ${testLmdb}

echo "All Done.."