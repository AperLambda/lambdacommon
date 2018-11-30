#!/bin/sh

# Some color variables.
RED='\033[0;91m'
NC='\033[0m'

sh ./clean_workspace.sh

echo "cd to build directory"

mkdir -p build/
cd build/
echo "Building project..."
cmake ..
if [ $? -ne 0 ]; then
echo "${RED}Error: CMake doesn't exit with success! Cleaning...${NC}"
cd ..
sh ./clean_workspace.sh
else
make
if [ $? -ne 0 ]; then
echo "${RED}Error: Make doesn't exit with success! Cleaning...${NC}"
cd ..
sh ./clean_workspace.sh
fi
fi

cd ..