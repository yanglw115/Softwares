#!/usr/bin/env sh

CaffeRoot=/home/yangliwei/caffe/caffe-master
ProjectDir=myProjects/ageAndGender
DataDir=${CaffeRoot}/${ProjectDir}/data

TrainDataList=${DataDir}/train.txt
TestDataList=${DataDir}/test.txt

trainLmdbName=image_train_lmdb
testLmdbName=image_test_lmdb
trainLmdb=${DataDir}/${trainLmdbName}
testLmdb=${DataDir}/${testLmdbName}

echo "Create train lmdb.."
rm -rf ${trainLmdb}


#convert_imageset是Caffe提供的图像转换工具，用于将训练图像集和验证图像集转换成Caffe方便处理的lmdb或leveldb的数据集。
#命令行使用格式：convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME 
#参数说明：
#	gray：bool类型，默认为false，如果设置为true，则代表将图像当做灰度图像来处理，否则当做彩色图像来处理
#	shuffle：bool类型，默认为false，如果设置为true，则代表将图像集中的图像的顺序随机打乱
#	backend：string类型，可取的值的集合为{"lmdb", "leveldb"}，默认为"lmdb"，代表采用何种形式来存储转换后的数据
#	resize_width：int32的类型，默认值为0，如果为非0值，则代表图像的宽度将被resize成resize_width
#	resize_height：int32的类型，默认值为0，如果为非0值，则代表图像的高度将被resize成resize_height
#	check_size：bool类型，默认值为false，如果该值为true，则在处理数据的时候将检查每一条数据的大小是否相同
#	encoded：bool类型，默认值为false，如果为true，代表将存储编码后的图像，具体采用的编码方式由参数encode_type指定
#	encode_type：string类型，默认值为""，用于指定用何种编码方式存储编码后的图像，取值为编码方式的后缀（如'png','jpg',...)


#下面的${DataDir}代表的是图片数据存在的路径，而${TrainDataList}文件里面的内容对应的图片路径必须是${DataDir}的相对路径，并且中间的"/"连接符号不能少
convert_imageset --shuffle --resize_height=256 --resize_width=256 ${DataDir} ${TrainDataList} ${trainLmdb}

echo "Create test lmdb.."
rm -rf ${testLmdb}
convert_imageset -shuffle --resize_width=256 --resize_height=256 ${DataDir} ${TestDataList} ${testLmdb}

echo "All Done.."