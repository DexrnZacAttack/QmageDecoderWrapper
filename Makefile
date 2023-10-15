CXX=g++
CFLAGS=-Wall -Wextra -Iinclude/ -L /system/lib -std=c++17
BUILD_DIR=bin

%.o: %.cpp
	$(CXX) -o ${BUILD_DIR}/$@ $< $(CFLAGS)

build: main.o

run:
	./${BUILD_DIR}/main.o