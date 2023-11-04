CC=gcc
CXX=g++
CFLAGS+=-Wall -Wextra -Iinclude/ -Og
CXXFLAGS+=-Wall -Wextra -Iinclude/ -std=c++17 -Og
LDFLAGS+=-Llib/ -lQmageDecoder -lm -fuse-ld=lld
BUILD_DIR=bin

all: build

build: $(BUILD_DIR)/qmdecoder

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/qmdecoder: $(BUILD_DIR)/qmdecoder.o $(BUILD_DIR)/image_helper.o
	$(CXX) -o $@ $^ $(LDFLAGS)

run: $(BUILD_DIR)/qmdecoder
	cd ${BUILD_DIR} && LD_LIBRARY_PATH="../lib/:${LD_LIBRARY_PATH}" ./qmdecoder ../examples/bootsamsung.qmg

clean:
	rm -f ${BUILD_DIR}/qmdecoder.o $(BUILD_DIR)/image_helper.o ${BUILD_DIR}/qmdecoder
