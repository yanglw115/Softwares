#!/usr/bin/env sh 

CaffeRoot=/home/yangliwei/caffe/caffe-master

ProjectDir=myProjects/ageAndGender
Data=${CaffeRoot}/${ProjectDir}/data

TrainData=${Data}/train
TestData=${Data}/test

TrainDataList=${Data}/train.txt
TestDataList=${Data}/test.txt


##--data
##-----|train
##----------|group1
##----------------|0
##----------------|1
##----------|group2
##----------------|0
##----------------|1
##----------......
##-----|test
##---------|0
##---------|1

echo "Create train.txt..."
rm -rf ${TrainDataList}

for group in `ls ${TrainData}` #overview train data directory
do
    groupDir=${TrainData}/${group}
    if [ -d ${groupDir} ];then
		for class in `ls ${groupDir}` #overview group directory
		do
			classDir=${groupDir}/${class}
			if [ -d ${classDir} ];then
				for element in `ls ${classDir}`
				do
					elementDir=${classDir}/${element}
					echo ${elementDir}
					if [ -s ${elementDir} ];then
						echo "${elementDir} ${class}" >> ${TrainDataList}
					fi
				done
			fi	
		done
    fi
done

echo "Create test.txt..."
rm -rf ${TestDataList}

echo "test data dir: " ${TestData}
for class in `ls ${TestData}`
do
	classDir=${TestData}/${class}
	echo ${classDir}
	if [ -d ${classDir} ];then
		for element in `ls ${classDir}`
		do
			echo ${elementDir}
			elementDir=${classDir}/${element}
			if [ -s ${elementDir} ];then
				echo "${elementDir} ${class}" >> ${TestDataList}
			fi
		done
	fi
done

echo "All done"
