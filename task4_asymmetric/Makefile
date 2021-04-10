UNAME_S=$(shell uname -s)

EXECUTABLE=task4
BUILD_DIR=build
SOURCE_DIR=src

MACOS_OPENSSL_DIR=/opt/homebrew/opt/openssl@1.1

CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -std=c++14 -O2

ifeq ($(UNAME_S), Darwin) # macOS
LD_FLAGS=-L$(MACOS_OPENSSL_DIR)/lib -I$(MACOS_OPENSSL_DIR)/include -lcrypto
else # Linux
LD_FLAGS=-lcrypto
endif

all: $(EXECUTABLE)

deps:
	g++ -MM *.cpp > Makefile.d

.PHONY: test
test: $(EXECUTABLE)
	./task4

$(EXECUTABLE): $(BUILD_DIR)/main.o
	$(LD) -o $(EXECUTABLE) $^ $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(BUILD_DIR)

# Dependencies:
-include Makefile.d
