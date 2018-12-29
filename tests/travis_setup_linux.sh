#!/usr/bin/env bash
echo "Installing compile dependencies..."
sudo apt-get update -y -qq
sudo apt-get install -y -qq libyajl-dev libxml2-dev libxqilla-dev
if [ "$CXX" = "clang++" ]; then
    sudo apt-get install -y  -qq libc++-dev;
fi