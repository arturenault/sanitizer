.PHONY: default compile build test exec clean format all
default: build

CXX			= clang++
CC			= clang++
CXXFLAGS	= -g -Wall --std=c++11 $(INCLUDES)
LDFLAGS		= -g
LDLIBS		= 
SRC_FILES	= $(shell find . -type f \( -name "*.cc" -or -name "*.h" \))
INCLUDES	= 

compile: sanitizer

build: compile

sanitizer: sanitizer.o

format:
	for file in $(SRC_FILES) ; do \
		clang-format --style=Google -i $$file ; \
	done

clean: 
	rm -f *.o sanitizer

all: clean default
