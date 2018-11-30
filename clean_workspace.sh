#!/bin/sh

echo "Checking build directories..."

if [ -d "build" ]; then
echo "Cleaning build directory..."
rm -rdv build/
fi
if [ -d "cmake-build-debug" ]; then
echo "Cleaning cmake-build-debug directory..."
rm -rdv cmake-build-debug/
fi