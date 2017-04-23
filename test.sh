#!/usr/bin/env bash

# Some color variables
RED='\033[0;91m'
NC='\033[0m'

if [ ! -d "build" ]; then
sh ./build.sh
fi

if [ ! -d "build" ]; then
echo "${RED}Cannot test AperCommon: Cannot build the library.${NC}"
return 1;
fi

cd build/
cp apercommon/libapercommon.so src/test
cd src/test
./apercommon_test