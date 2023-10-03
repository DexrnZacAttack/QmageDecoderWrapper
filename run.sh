#!/bin/sh
rm -f bin/main
LD_LIBRARY_PATH=.:/system/lib:$LD_LIBRARY_PATH
g++ main.cpp -Iinclude/ -L /system/lib -o bin/main
chmod +x bin/main
cd bin
./main
cd ../