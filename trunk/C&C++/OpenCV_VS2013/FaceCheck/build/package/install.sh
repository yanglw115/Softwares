#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

if [ `id -u` -ne 0 ]; then
    echo "Please use 'root' user, or run by 'sudo $0'"
    exit 1
else
    echo "Start to install face parser tools and libraries. It will take a few minutes, please wait..."
fi

PWD=$(pwd)
NamePackage=package.tar.gz
NameStdcpp6=libstdc++.so.6
NameStdcppFull=libstdc++.so.6.0.24
NameJNILib=libCureFaceParser.so
NameFaceLandmarks=shape_predictor_68_face_landmarks.dat

DirInstall=/usr/local/FaceParser
DirStdcppInstall=/usr/lib64

if [ -d ${DirInstall} ]
then
	rm ${DirInstall}/* -rf
else
	mkdir ${DirInstall}
fi

sed -n -e "1,/^###End###$/!p" "$0" > ${DirInstall}/${NamePackage}

cd ${DirInstall}
tar xvf ${NamePackage}

cp -f ${NameStdcppFull} ${DirStdcppInstall}
cd ${DirStdcppInstall}
rm -rf ${NameStdcpp6}
ln -sv ${NameStdcppFull} ${NameStdcpp6}

echo "Congratulations! Install face parser success!"

exit 0

###End###


