#!/bin/sh
LD_LIBRARY_PATH=.:/system/lib:$LD_LIBRARY_PATH
clang++ main.cpp -Iinclude/ -L /system/lib -lc -o main
chmod +x main
./main