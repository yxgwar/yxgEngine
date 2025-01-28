@echo off
echo Y|del build
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make -j8
PAUSE