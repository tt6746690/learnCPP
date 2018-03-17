COMPILER = clang++
CXXFLAGS = -std=c++1z -stdlib=libc++ -Wall -g
OUTFILES = die6sided

all : $(OUTFILES)

%: %.o
	$(COMPILER) $(CXXFLAGS) -o die6sided $^

%.o : %.c 
	$(COMPILER) $(CXXFLAGS) -c $<

clean:
	rm ./*.o $(OUTFILES) log