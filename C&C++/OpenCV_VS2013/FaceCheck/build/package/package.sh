#!/bin/bash

PWD=$(pwd)
NameLibsPackage=libs.tar.gz
DirLibs=${PWD}/../libs
DirLibStdcpp=${PWD}/../libstdc++.so.6.0.24
DirLibJNILib=${PWD}../../libCureFaceParser.so

rm -rf ${DirLibs}/${NameLibsPackage}

set -o nounset  #脚本中禁止使用未定义的变量。或使用set -u
set -o errexit  #脚本中任意一行执行错误时则退出。或使用set -e




