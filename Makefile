CXX=g++
CFLAGS=-Wall -Wextra -Iinclude/ -Llib/ -std=c++17
BUILD_DIR=bin

%.o: %.cpp
	$(CXX) -o ${BUILD_DIR}/$@ $< $(CFLAGS)

build: main.o

run:
	cd ${BUILD_DIR} && ./main.o

clean:
	rm -f ${BUILD_DIR}/main.o