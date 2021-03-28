UNAME_S=$(shell uname -s)

EXECUTABLE=task3
BUILD_DIR=build
SOURCE_DIR=src

CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -std=c++14 -O2 -g -fsanitize=address

ifeq ($(UNAME_S), Darwin) # macOS
LD_FLAGS=-L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -lcrypto -g -fsanitize=address
else # Linux
LD_FLAGS=-lcrypto
endif

all: $(EXECUTABLE)

deps:
	g++ -MM *.cpp > Makefile.d

.PHONY: test
test: $(EXECUTABLE)
	./$(EXECUTABLE) -e ecb obrazek_tga/Mad_scientist.tga

$(EXECUTABLE): $(BUILD_DIR)/block.o $(BUILD_DIR)/AESEncryptor.o $(BUILD_DIR)/AESDecryptor.o $(BUILD_DIR)/TGAReader.o
	$(LD) -o $(EXECUTABLE) $^ $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(BUILD_DIR)

# Dependencies:
-include Makefile.d
