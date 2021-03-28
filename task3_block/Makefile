UNAME_S=$(shell uname -s)

EXECUTABLE=task3
BUILD_DIR=build

CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -std=c++14 -O2

ifeq ($(UNAME_S), Darwin) # macOS
LD_FLAGS=-L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -lcrypto
else # Linux
LD_FLAGS=-lcrypto
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD_DIR)/block.o
	$(LD) -o $(EXECUTABLE) $< $(LD_FLAGS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(BUILD_DIR)
	