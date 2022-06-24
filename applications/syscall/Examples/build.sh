#!/bin/bash

FILE_NAME=$1
BUILD_DIR="Build"

#check file exists
[ ! -f ${FILE_NAME} ] && { echo "file is not exist"; exit 1; }

#check is C source file
if [[ ${FILE_NAME} == *".cpp" ]]
then
	echo "Building C plus plus source file."
elif [[ ${FILE_NAME} == *".c" ]]
then
	echo "Building C source file."
else
	echo "Not C file, exit."
	exit 1
fi

[ ! -d ${BUILD_DIR} ] && mkdir "${BUILD_DIR}"

gcc ${FILE_NAME} -o ${BUILD_DIR}/$(cut -d '.' -f1 <<< ${FILE_NAME}).o

