CXX=g++
CFLAGS=-Wall -Wextra -Iinclude/ -Llib/ -lQmageDecoder -lm -std=c++17 -Og
BUILD_DIR=bin

%.o: %.cpp
	$(CXX) -o ${BUILD_DIR}/$@ $< $(CFLAGS)

build: main.o

run:
	cd ${BUILD_DIR} && LD_LIBRARY_PATH="../lib/:${LD_LIBRARY_PATH}" ./main.o ../examples/bootsamsung.qmg

clean:
	rm -f ${BUILD_DIR}/main.o