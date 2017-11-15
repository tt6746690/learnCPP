COMPILER = clang++
# CPP_FILES = $(wildcard ./*/*.cpp)
# EXECUTABLES = $(CPP_FILES:.cpp=)
SRC_DIR := ./array
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) ./main.cpp
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) ./main.o

CXXFLAGS = -std=c++1z -stdlib=libc++ -Wall -Iinclude
HEADERS = ./include/includes.h ./include/catch.hpp

# all: $(EXECUTABLES)
# all: ./array/88_merge_sorted_array.o

# test: $(MAIN)
# 	$(COMPILER) -o test $(CF_FLAGS)

# %.o: %.cpp $(HEADERS)
# 	$(COMPILER) -c $@ $< $(CF_FLAGS)


testing: $(OBJ_FILES) 
	$(COMPILER) -o $@ $^ $(CXXFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(COMPILER) -c -o $@ $< $(CXXFLAGS) 

