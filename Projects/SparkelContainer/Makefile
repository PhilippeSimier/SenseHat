# Makefile for building the SparkelContainer
# with SenseHat.

# Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/include

# Library path (Update this with the library installation folder).
LIBRARY_PATH = /usr/lib

# List of source files
SOURCES += IsolatedContainer.cpp SparkelContainer.cpp

#List of object files
OBJECTS += $(SOURCES:.cpp=.o)

all: SparkelContainer

#Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ -I $(HEADERS_PATH) -std=c++0x -c $^ -o $@
	@echo

#Building SparkelContainer executable
SparkelContainer: SparkelContainer.o IsolatedContainer.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

# Uninstall the exemples from the system
uninstall:
	rm -f  SparkelContainer



.PHONY: clean
clean:
	rm -f *.o
