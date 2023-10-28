# Set the compiler and flags
CXX=g++
CXXFLAGS=-g -Wall
# Set the linker flags
LD_FLAGS=
# Configure project directories (src & hdrs manually created)
SRC_DIR=src
HDR_DIR=src\hdrs
OBJ_DIR=obj
BIN_DIR=bin
# Configure project files in associated directories utilizing macro function if Windows
HDR_FILES=$(wildcard $(HDR_DIR)\*.h)
OBJ_FILES=$(OBJ_DIR)\arg_validate.o $(OBJ_DIR)\utils.o $(OBJ_DIR)\what_in_the_shell.o
EXE_FILE=$(BIN_DIR)\WhatInTheShell.exe

# Executable recipe
$(EXE_FILE): $(OBJ_DIR) $(BIN_DIR) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(LD_FLAGS) -o $@

# Object files recipe
$(OBJ_DIR)\\%.o: $(SRC_DIR)\%.cpp $(HDR_FILES)
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