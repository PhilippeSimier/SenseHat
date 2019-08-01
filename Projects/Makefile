# Makefile for building the Projects Folder
# with SenseHat.

# Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/include

# Library path (Update this with the library installation folder).
LIBRARY_PATH = /usr/lib

# List of source files
SOURCES += pixelBlink.cpp digitalClock.cpp

#List of object files
OBJECTS += $(SOURCES:.cpp=.o)

all: pixelBlink digitalClock

#Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ -I $(HEADERS_PATH) -std=c++0x -c $^ -o $@
	@echo

#Building  pixelBlink executable
pixelBlink: pixelBlink.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building  pixelBlink executable
digitalClock: digitalClock.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

# Uninstall the exemples from the system
uninstall:
	rm -f  pixelBlink digitalClock



.PHONY: clean
clean:
	rm -f *.o
