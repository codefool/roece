#
# Makefile for ROECE
#
# This lives at https://github.com/codefool/ROECE
#
INC_DIR := ./include
SRC_DIR := ./src
OBJ_DIR := ./obj
LIB_DIR := ./lib
TOOLS_DIR := ./tools
LIB_NAME := $(LIB_DIR)/libroece
LIB_INC := /usr/local/libcf/lib/libcf

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HDR := $(wildcard $(INC_DIR)/*.h)

CC := g++
CFLAGS := -g -std=c++2a -I$(INC_DIR) -I/usr/local/libcf/include
ARC := ar
AFLAGS := rvs

# garth : garth.cpp garth.h $(HDR) $(LIB_NAME)
# 	$(CC) $(CFLAGS) garth.cpp -L/usr/lib/x86_64-linux-gnu -L/usr/local/libcf/lib/libcf $(LIB_NAME) -o $@

$(LIB_NAME) : $(OBJ)
	$(ARC) $(AFLAGS) $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

build-ver:
	sed -ri 's/(.*)(ver_build = )([0-9]*);/echo "\1\2$$((\3+1));"/ge' $(INC_DIR)/buildinfo.h

clean:
	-rm $(OBJ_DIR)/*.o $(LIB_NAME)

clean-test:
	rm -rf /tmp/work/ttemp/ /tmp/work/positions/ /tmp/work/roots/ /tmp/work/domain/

.PHONY: build-ver

test: $(TOOLS_DIR)/test.cpp build-ver $(LIB_NAME)
	$(CC) $(CFLAGS) $(TOOLS_DIR)/test.cpp -I. -L/usr/lib/x86_64-linux-gnu $(LIB_INC) $(LIB_NAME) -o test

rd128: $(TOOLS_DIR)/rd128.cpp
	$(CC) $(CFLAGS) $(TOOLS_DIR)/rd128.cpp -L/usr/lib/x86_64-linux-gnu -o rd128
