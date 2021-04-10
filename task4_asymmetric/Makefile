UNAME_S=$(shell uname -s)

EXECUTABLE=task4
BUILD_DIR=build
SOURCE_DIR=src

MACOS_OPENSSL_DIR=/opt/homebrew/opt/openssl@1.1

CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -std=c++14 -O2 -g

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
	./task4 -e pubkey.pem Makefile Makefile.enc
	./task4 -d privkey.pem Makefile.enc Makefile.dec
	diff Makefile Makefile.dec

$(EXECUTABLE): $(BUILD_DIR)/main.o $(BUILD_DIR)/Encryptor.o $(BUILD_DIR)/Decryptor.o
	$(LD) -o $(EXECUTABLE) $^ $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(BUILD_DIR) *.dec *.enc

# Dependencies:
-include Makefile.d
