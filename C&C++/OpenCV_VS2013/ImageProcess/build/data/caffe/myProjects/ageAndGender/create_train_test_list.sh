#!/usr/bin/env sh 

CaffeRoot=/home/yangliwei/caffe/caffe-master

ProjectDir=myProjects/ageAndGender
DataDir=${CaffeRoot}/${ProjectDir}/data

TrainData=train
TestData=test

TrainDataList=${DataDir}/train.txt
TestDataList=${DataDir}/test.txt


##--data
##-----|train
##----------|0
##----------|1
##----------......
##-----|test
##---------|0
##---------|1
##----------......

echo "Create train.txt..."
rm -rf ${TrainDataList}

#这里只需要分类即可，不需要分组再分类。因为在使用convert_imageset方法的时候加上--shuffle参数会自动随机排序
for class in `ls ${DataDir}/${TrainData}` #overview train data directory
do
    classDir=${DataDir}/${TrainData}/${class}
    if [ -d ${classDir} ];then
		for element in `ls ${classDir}`
		do
			elementDir=${classDir}/${element}
			echo ${elementDir}
			if [ -s ${elementDir} ];then
				echo "/${TrainData}/${class}/${element} ${class}" >> ${TrainDataList}
			fi
		done
    fi
done

echo "Create test.txt..."
rm -rf ${TestDataList}

for class in `ls ${DataDir}/${TestData}`
do
	classDir=${DataDir}/${TestData}/${class}
	if [ -d ${classDir} ];then
		for element in `ls ${classDir}`
		do
			echo ${elementDir}
			elementDir=${classDir}/${element}
			if [ -s ${elementDir} ];then
				echo "/${TestData}/${class}/${element} ${class}" >> ${TestDataList}
			fi
		done
	fi
done

echo "All done"
