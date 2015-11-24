.PHONY: default compile build test exec clean format all
default: build

CXX			= clang++
CC			= clang++
CXXFLAGS	= -g -Wall --std=c++11 $(INCLUDES)
LDFLAGS		= -g
LDLIBS		= 
SRC_FILES	= $(shell find . -type f \( -name "*.cc" -or -name "*.h" \))
UNI				= aur2103
INCLUDES	= -DUNI=\"$(UNI)\" 

compile: sanitizer

build: compile

sanitizer: sanitizer.o

test: build
	rm -rf sandbox
	mkdir sandbox
	for file in tests/* ; do \
		cd sandbox 2>/dev/null ;  cat ../$$file | ../sanitizer ; \
	done

format:
	for file in $(SRC_FILES) ; do \
		clang-format --style=Google -i $$file ; \
	done

clean: 
	rm -rf *.o sanitizer sandbox

all: clean default
