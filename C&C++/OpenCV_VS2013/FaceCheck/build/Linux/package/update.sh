#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

echo -e "\n"
# 需要用root权限
if [ `id -u` -ne 0 ]; then
    echo "Please use 'root' user, or run by 'sudo $0'"
    exit 1
else
    echo "Start to update face parser tools and libraries, please wait..."
fi

PWD=$(pwd)
NameProject=FaceParser
NamePackageUpdate=update.tar.gz
NameJNILib=libCureFaceRecognition.so

DirInstall=/usr/local/${NameProject}
DirInstallLibs=/usr/local/${NameProject}/libs

if [ -d ${DirInstall} ]
then
	echo "..."
else
	echo "Oops! You may not install face parser tools or libraries before, cannot update!(Error)"
	exit -1;
fi

# 提取安装包
sed -n -e '1,/^###End###$/!p' $0 > ${DirInstall}/${NamePackageUpdate}

# 解压缩安装包
cd ${DirInstall}
tar xvf ${NamePackageUpdate}

# 更新faceParser库
cp -f ${NameJNILib} ${DirInstallLibs}

# 删除无用的文件
rm -rf ${DirInstall}/${NamePackageUpdate}
rm -rf ${NameJNILib}

echo -e "\nCongratulations! Update face parser success!\n\n"

exit 0
#注意下面一行必须有且只能有一个换行符号。理论上讲不需要，但是实际应用出了问题
###End###
