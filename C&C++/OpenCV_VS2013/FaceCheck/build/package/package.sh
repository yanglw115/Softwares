#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

PWD=$(pwd)
NamePackage=package.tar.gz
NameStdcpp=libstdc++.so.6.0.24
NameJNILib=libCureFaceParser.so
NameFaceLandmarks=shape_predictor_68_face_landmarks.dat
NameInstallShell=install.sh
NameInstallBin=FaceParserInstall.bin
DirLibs=${PWD}/../libs
DirLibStdcpp=${PWD}/../${NameStdcpp}
DirLibJNILib=${PWD}/../../${NameJNILib}
DirFaceLandmarks=${PWD}/../../${NameFaceLandmarks}

read -p "Do you have copyed opencv and dlibs libraries to their directory? [Y/N]: " REPLY

if [ -z ${REPLY} ] || [ ${REPLY}x = "Y"x ] || [ ${REPLY}x = "y"x ]
then
	echo "Start to package..."
else
	echo "Failed!"
	exit
fi
rm -rf ${PWD}/${NamePackage}
cp -f ${DirLibStdcpp} ${DirLibs}
cp -f ${DirLibJNILib} ${DirLibs}
cp -f ${DirFaceLandmarks} ${DirLibs}

tar czvf ${NamePackage} ${DirLibs}/*

rm ${DirLibs}/${NameStdcpp}
rm ${DirLibs}/${NameJNILib}
rm ${DirLibs}/${NameFaceLandmarks}

cat ${PWD}/${NameInstallShell} ${PWD}/${NamePackage} > ${PWD}/${NameInstallBin}
chmod a+x ${PWD}/${NameInstallBin}

echo "Package success!"




