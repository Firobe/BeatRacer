COMPILER = g++
ADD_CFLAGS = -Wall -g -std=c++11
LIBS = glu openal freealut ftgl glfw3
NO_PKG_LIBS = 
CXXFLAGS = `pkg-config --static --cflags $(LIBS)` $(ADD_CFLAGS)
LINKS = `pkg-config --static --libs $(LIBS)` $(NO_PKG_LIBS)
SOURCES = $(wildcard src/*.cpp)
EXEC_NAME = btrc
OBJECTS = $(SOURCES:.cpp=.o)
.PRECIOUS : %.o
.PHONY : clean
.PHONY : run

all: $(OBJECTS)
	$(COMPILER) -o $(EXEC_NAME) $(OBJECTS) $(LINKS)

%.o : %.c
	$(COMPILER) -c $(CXXFLAGS) $< -o $@

run: all
	./$(EXEC_NAME)

clean:
	rm -f $(OBJECTS) $(EXEC_NAME)
