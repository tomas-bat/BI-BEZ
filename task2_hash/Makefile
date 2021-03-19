# This Makefile should work on macOS (x86_64), macOS (arm), Linux (x86_64)

UNAME_S = $(shell uname -s)
ARCH = $(shell uname -p)

CXX = g++
CXX_FLAGS = -Wall -pedantic -std=c++14 -O2

ifeq ($(UNAME_S), Darwin) # macOS (requires this link specification, don't know why)
LD_FLAGS = -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -lcrypto
ifeq ($(ARCH), arm) #macOS Apple Silicon (compile for x86_64, openssl lib not compatible with ARM?)
ARCH = arch -x86_64
else
ARCH =
endif
else # Linux
LD_FLAGS = -lcrypto
ARCH =
endif

all: hash

hash: hash.o
	$(ARCH) $(CXX) -o task2 $< $(LD_FLAGS)

%.o: %.cpp
	$(ARCH) $(CXX) -c -o $@ $< $(CXX_FLAGS)

.PHONY: clean
clean:
	rm -f *.o task2
	