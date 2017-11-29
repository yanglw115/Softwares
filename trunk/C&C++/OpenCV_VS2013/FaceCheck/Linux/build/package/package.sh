#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

PWD=$(pwd)
NamePackage=package.tar.gz
NameStdcpp=libstdc++.so.6.0.24
NameJNILib=libCureFaceRecognition.so
NameInstallShell=install.sh
NameInstallBin=FaceParserInstall

DirLibs=${PWD}/../libs
DirLibStdcpp=${PWD}/../${NameStdcpp}
DirLibJNILib=${PWD}/../../${NameJNILib}
DirData=${PWD}/../data

echo -e "\n"
# 提示判断opencv和dlib库是否都已经准备好并放在预定目录
read -p "Do you have copyed opencv and dlibs libraries to their directory? [Y/N]: " REPLY

if [ -z ${REPLY} ] || [ ${REPLY}x = "Y"x ] || [ ${REPLY}x = "y"x ]
then
	echo "Start to package..."
else
	echo "Failed!"
	exit
fi

rm -rf ${PWD}/${NamePackage}
rm -rf ${PWD}/${NameInstallBin}*

# 创建压缩包，压缩打包除安装脚本以外的所有文件
cp -f ${DirLibJNILib} ${DirLibs}
tar czvf ${NamePackage} ${DirLibs} ${DirLibStdcpp} ${DirData}

# 将安装脚本和安装包cat到一起，生成.bin安装文件
cat ${NameInstallShell} ${NamePackage} > ${NameInstallBin}
chmod a+x ${NameInstallBin}
mv ${NameInstallBin} ${NameInstallBin}.$(date "+%Y%m%d_%H.%M.%S").bin

rm -rf ${NamePackage}
rm -rf ${DirLibs}/${NameJNILib}

echo -e "Package success!\n\n"
