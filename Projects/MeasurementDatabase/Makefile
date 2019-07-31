# Makefile for building the measureToDB
# with SenseHat.

# Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/include

# Library path (Update this with the library installation folder).
LIBRARY_PATH = /usr/lib

# List of source files
SOURCES += measureToDB.cpp

#List of object files
OBJECTS += $(SOURCES:.cpp=.o)

all: measureToDB

#Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ -I $(HEADERS_PATH) -std=c++0x -c $^ -o $@
	@echo



#Building compass executable
measureToDB: measureToDB.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l mysqlclient -l RTIMULib
	@echo

# Uninstall the exemples from the system
uninstall:
	rm -f  measureToDB



.PHONY: clean
clean:
	rm -f *.o
