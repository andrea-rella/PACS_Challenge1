#folder with include and lib directories for json and muparserX
#@note if you  add export PACS_ROOT=/path/to/your/libs in your .bashrc file, 
#you can use the following line
LIBS_ROOT = $(PACS_ROOT)

CXX      ?= g++

CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -Wno-conversion-null -Wno-deprecated-declarations -I${LIBS_ROOT}/include 
LDFLAGS ?= -L${LIBS_ROOT}/lib


EXEC = main
LDFLAGS ?= -L${LIBS_ROOT}/lib
LIBS  ?= -lmuparser


all: $(EXEC)

%.o: %.cpp 
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXEC): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@


clean:
	$(RM) *.o

distclean: clean
	$(RM) *~ $(EXEC)

	
	
	