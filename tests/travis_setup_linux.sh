#!/usr/bin/env bash
echo "Installing compile dependencies..."
sudo apt update -y -qq
sudo apt install -y -qq libyajl-dev libxml2-dev libxqilla-dev
if [ "$CXX" = "clang++" ]; then sudo apt install -y  -qq libstdc++-6-dev; fi
if [ "$CXX" = "g++" ]; then
    sudo apt install -y -qq g++-7
    export CXX="g++-7" CC="gcc-7"
fi