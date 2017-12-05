#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

PWD=$(pwd)
NameJNILib=libCureFaceRecognition.so
NameUpdateShell=update.sh
NamePackageUpdate=update.tar.gz
NameUpdateBin=FaceParserUpdate
NameProject=project

DirLibJNILib=${PWD}/../${NameProject}/${NameJNILib}

echo -e "\n"

rm -rf ${PWD}/${NamePackageUpdate}
rm -rf ${PWD}/${NameUpdateBin}*

# 创建压缩包，压缩打包除安装脚本以外的所有文件
cp ${DirLibJNILib} ${PWD}
tar czvf ${NamePackageUpdate} ${NameJNILib}
rm -rf ${NameJNILib}

# 将安装脚本和安装包cat到一起，生成.bin安装文件
cat ${NameUpdateShell} ${NamePackageUpdate} > ${NameUpdateBin}
chmod u+x ${NameUpdateBin}
mv ${NameUpdateBin} ${NameUpdateBin}.$(date "+%Y%m%d_%H.%M.%S").bin

rm -rf ${NamePackageUpdate}

echo -e "Package update success!\n\n"

