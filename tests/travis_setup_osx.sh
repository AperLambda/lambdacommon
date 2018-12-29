#!/usr/bin/env bash
echo "Installing compile dependencies..."
brew update
if [ "$CXX" = "g++" ]; then
    echo "Installing GCC..."
    brew install gcc
fi