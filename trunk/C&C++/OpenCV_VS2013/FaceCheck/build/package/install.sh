#!/bin/bash

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e

echo -e "\n"
# 需要用root权限
if [ `id -u` -ne 0 ]; then
    echo "Please use 'root' user, or run by 'sudo $0'"
    exit 1
else
    echo "Start to install face parser tools and libraries. It will take a few minutes, please wait..."
fi

PWD=$(pwd)
NameProject=FaceParser
NamePackage=package.tar.gz
NameStdcpp6=libstdc++.so.6
NameStdcppFull=libstdc++.so.6.0.24
NameFaceLandmarks=shape_predictor_68_face_landmarks.dat

DirInstall=/usr/local/${NameProject}
DirInstallLibs=/usr/local/${NameProject}/libs
DirStdcppInstall=/usr/lib64
DirLDConfig=/etc/ld.so.conf.d

if [ -d ${DirInstall} ]
then
	rm ${DirInstall}/* -rf
else
	mkdir ${DirInstall}
fi

# 提取安装包
sed -n -e '1,/^###End###$/!p' $0 > ${DirInstall}/${NamePackage}

# 解压缩安装包
cd ${DirInstall}
tar xvf ${NamePackage}

# 更新系统GLBC版本
cp -f ${NameStdcppFull} ${DirStdcppInstall}
cd ${DirStdcppInstall}
rm -rf ${NameStdcpp6}
ln -sv ${NameStdcppFull} ${NameStdcpp6}

# 删除无用的文件
rm -rf ${DirInstall}/${NamePackage}
rm -rf ${DirInstall}/${NameStdcppFull}

# 添加系统动态库链接路径
echo ${DirInstallLibs} > ${DirLDConfig}/${NameProject}
ldconfig

echo -e "\nCongratulations! Install face parser success!\n\n"

exit 0
#注意下面一行必须有且只能有一个换行符号
###End###
