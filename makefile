# Set the compiler and flags
CXX=g++
CXXFLAGS=-g -Wall
# Set the linker flags
LD_FLAGS=
# Configure project directories
SRC_DIR=src
HDR_DIR=$(SRC_DIR)\hdrs
OBJ_DIR=obj
BIN_DIR=bin
# Configure project files in associated directories
SRC_FILES=$(wildcard $(SRC_DIR)\*.cpp)
HDR_FILES=$(wildcard $(HDR_DIR)\*.h)
OBJ_FILE=$(OBJ_DIR)\what_in_the_shell.o
EXE_FILE=$(BIN_DIR)\WhatInTheShell?.exe

# Executable recipe
$(EXE_FILE): $(OBJ_DIR) $(BIN_DIR) $(OBJ_FILE)
	$(CXX) $(CXXFLAGS) $(OBJ_FILE) $(LD_FLAGS) -o $@

# Object file recipe
$(OBJ_FILE): $(SRC_FILES) $(HDR_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensures dir creation of obj & bin folders
$(OBJ_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

# Used to clean up old bins & object files before compilation
clean:
	$(RM) /S $(BIN_DIR)/* $(OBJ_DIR)/*

# Needed for Clion IDE
all: $(EXE_FILE)