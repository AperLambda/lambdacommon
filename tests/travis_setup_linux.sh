#!/usr/bin/env bash
echo "Installing compile dependencies..."
sudo apt-get update -y -qq
sudo apt-get install -y -qq libyajl-dev libxml2-dev libxqilla-dev
if [ "$CXX" = "clang++" ]; then
    sudo apt-get install -y  -qq libc++-dev;
fi
if [ "$CXX" = "g++" ]; then
    echo "Installing GCC..."
    sudo apt-get install -y -qq g++-7
    export CXX="g++-7" CC="gcc-7"
fi