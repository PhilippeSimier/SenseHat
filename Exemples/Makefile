# Makefile for building the examples distributed
# with SenseHat.

# Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/include

# Library path (Update this with the library installation folder).
LIBRARY_PATH = /usr/lib

# List of source files
SOURCES += caractere.cpp clock.cpp exemple.cpp niveau.cpp secouer.cpp text.cpp

#List of object files
OBJECTS += $(SOURCES:.cpp=.o)

all: caractere compass clock exemple niveau secouer text

#Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ -I $(HEADERS_PATH) -std=c++0x -c $^ -o $@
	@echo

#Building caractere executable
caractere: caractere.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building compass executable
compass: compass.o
	@@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo 

#Building exemple executable
exemple: exemple.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building clock executable
clock: clock.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building secouer executable
secouer: secouer.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building text executable
text: text.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

#Building niveau executable
niveau: niveau.o
	@echo Building target file: $@.
	g++ -L$(LIBRARY_PATH) -o $@ $^ -l SenseHat -l RTIMULib
	@echo

# Uninstall the exemples from the system
uninstall:
	rm -f  niveau text2 text secouer exemple caractere clock



.PHONY: clean
clean:
	rm -f *.o

