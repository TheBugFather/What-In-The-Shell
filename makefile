# Set the compiler and flags
CXX=g++
CXXFLAGS=-g -Wall
# Set the linker flags
LD_FLAGS=

# If the OS is Windows
ifeq ($(OS),Windows_NT)
	# Convert path separators for Windows
 	WIN_PATH=$(subst /,\,$1)
# If the OS is non-Windows
else
	# Retain the non-Windows forward slash format
	WIN_PATH=$1
endif

# Configure project directories (src & hdrs manually created)
SRC_DIR=src
HDR_DIR=src/hdrs
OBJ_DIR=obj
BIN_DIR=bin
# Configure project files in associated directories
SRC_FILES=$(wildcard $(call WIN_PATH,$(SRC_DIR)/*.cpp))
HDR_FILES=$(wildcard $(call WIN_PATH,$(HDR_DIR)/*.h))
OBJ_FILES=$(patsubst $(call WIN_PATH,$(SRC_DIR)/*.cpp),$(OBJ_DIR)/%.o,$(SRC_FILES))
EXE_FILE=$(BIN_DIR)/WhatInTheShell?.exe

# Executable recipe
$(EXE_FILE): $(OBJ_DIR) $(BIN_DIR) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(LD_FLAGS) -o $@

# Pattern rule for building object files
$(OBJ_FILES): $(SRC_FILES) $(HDR_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensures dir creation of obj & bin folders
$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Used to clean up old bins & object files before compilation
clean:
	$(RM) -r $(BIN_DIR)/* $(OBJ_DIR)/*

# Needed for Clion IDE
all: $(EXE_FILE)
