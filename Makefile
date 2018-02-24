#Makefile that builds SenseHat library.

#Project name
LIBRARY_NAME = libSenseHat

# Prefix
PREFIX = /usr

# List of source files
SOURCES += SenseHat.cpp

# List of object files
OBJECTS += $(SOURCES:.cpp=.o)

# Headers
HEADERS += $(SOURCES:.cpp=.h) font.h

# Define C++ compiler flags
#override CXXFLAGS = -O3 -Wall
override CXXFLAGS = -Wall

# Linker flags
LDFLAGS = -shared

all: $(LIBRARY_NAME).so

# Compile all of the source files
%.o: %.cpp
	@echo Compiling file: $<
	g++ $(CXXFLAGS) -c $< -o $@
	@echo

# Build the binary
$(LIBRARY_NAME).so: $(OBJECTS)
	@echo Building target file: $@.
	g++ $(LDFLAGS) -o $@ $^ -lm


# Install the library in $(prefix)
install:
	install -m 0644 $(LIBRARY_NAME).so $(PREFIX)/lib
	install -m 0644 SenseHat.h $(PREFIX)/include

# Uninstall the library from the system
uninstall:
	rm -f $(PREFIX)/lib/$(LIBRARY_NAME).so
	rm -f $(PREFIX)/include/SenseHat.h

.PHONY: clean
clean:
	rm $(LIBRARY_NAME).so $(OBJECTS)
