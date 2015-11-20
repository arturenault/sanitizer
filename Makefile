.PHONY: default compile build test exec clean format all
default: build

CXX			= clang++
CC			= clang++
CXXFLAGS	= -g -Wall --std=c++11 $(INCLUDES)
LDFLAGS		= -g
LDLIBS		= 
SRC_FILES	= $(shell find . -type f \( -name "*.cc" -or -name "*.h" \))
INCLUDES	= 

compile:

build: compile

sanitizer: sanitizer.o

clean: check
	rm -rf *.o addqueue showqueue rmqueue /usr/local/bin/addqueue /usr/local/bin/showqueue /usr/local/bin/rmqueue $(ROOT_DIR)

format:
	for file in $(SRC_FILES) ; do \
		clang-format --style=Google -i $$file ; \
	done

all: clean default
