COMPILER = clang++
CPPFLAGS = -std=c++1z -stdlib=libc++ -Wall -I.

CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

HEADERS = 4_pimpl.h

OBJS_4 = 4_pimpl.o 4.o

all: 4

4: $(OBJS_4) 
	$(COMPILER) $(CPPFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(COMPILER) $(CPPFLAGS) -c -o $@ $<

clean:
	rm ./*.o
	rm 4
