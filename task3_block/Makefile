UNAME_S=$(shell uname -s)

EXECUTABLE=task3
BUILD_DIR=build
SOURCE_DIR=src

MACOS_OPENSSL_DIR=/opt/homebrew/opt/openssl@1.1

CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -std=c++14 -O2

ifeq ($(UNAME_S), Darwin) # macOS
LD_FLAGS=-L$(MACOS_OPENSSL_DIR)/lib -I$(MACOS_OPENSSL_DIR)/include -lcrypto
else # Linux
LD_FLAGS=-lcrypto -g
endif

all: $(EXECUTABLE)

deps:
	g++ -MM *.cpp > Makefile.d

.PHONY: test
test: $(EXECUTABLE)
	rm -f obrazek_tga/Mad_scientist_cbc.tga obrazek_tga/Mad_scientist_cbc_dec.tga && \
	./$(EXECUTABLE) -e cbc obrazek_tga/Mad_scientist.tga && \
	./$(EXECUTABLE) -d cbc obrazek_tga/Mad_scientist_cbc.tga && \
	diff obrazek_tga/Mad_scientist.tga obrazek_tga/Mad_scientist_cbc_dec.tga

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
