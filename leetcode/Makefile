COMPILER = g++
CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp 
	$(COMPILER) -o $@ $<
