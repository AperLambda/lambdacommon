# λcommon

[![Website](https://img.shields.io/badge/Website-%CE%BBcommon-9B599A.svg?style=flat-square)](https://aperlambda.github.io/libraries/lambdacommon/)
![C++](https://img.shields.io/badge/language-C++-9B599A.svg?style=flat-square)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://raw.githubusercontent.com/AperLambda/lambdacommon/master/LICENSE)
[![Version](https://img.shields.io/github/release/AperLambda/lambdacommon.svg?style=flat-square)](https://github.com/AperLambda/lambdacommon/releases)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/981c6234700d413297724d7d759e2dec)](https://www.codacy.com/app/LambdAurora/lambdacommon?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AperLambda/lambdacommon&amp;utm_campaign=Badge_Grade)
[![Build Status](https://travis-ci.org/AperLambda/lambdacommon.svg?branch=master)](https://travis-ci.org/AperLambda/lambdacommon/)
[![Build status](https://ci.appveyor.com/api/projects/status/aufclhjsvecva6mk?svg=true)](https://ci.appveyor.com/project/LambdAurora/lambdacommon)
![Downloads](https://img.shields.io/github/downloads/AperLambda/lambdacommon/latest/total.svg?style=flat-square)
[![GitHub issues](https://img.shields.io/github/issues/AperLambda/lambdacommon.svg?style=flat-square)](https://github.com/AperLambda/lambdacommon/issues/)

A library written in C++ with common features.

Features: OS Detection, Terminal colors, string manipulations, filesystem manipulations, URI manipulations, and more!

[![Packaging status](https://repology.org/badge/vertical-allrepos/lambdacommon.svg)](https://repology.org/metapackage/lambdacommon)

## Installation

### Windows

Please install from the source.

### Linux

#### ArchLinux (and variants)

Just install [lambdacommon](https://aur.archlinux.org/packages/lambdacommon/) from AUR

#### Others

Please install from the source. 

### From source

Build the sources with CMake and make and install with `make install`, and keep the install manifest to allow the uninstallation with `make uninstall`. 

## Use in CMake

Use `Findlambdacommon.cmake` in [LambdaCMakeModules](https://github.com/AperLambda/LambdaCMakeModules.git) to find λcommon on your computer.