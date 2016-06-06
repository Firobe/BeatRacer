COMPILER = c++
ADD_CFLAGS = -Wall -g -std=c++11 -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function
LIBS = glu openal glfw3
NO_PKG_LIBS = -lAntTweakBar
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

%.o : %.cpp %.h
	$(COMPILER) -c $(CXXFLAGS) $< -o $@

run: all
	./$(EXEC_NAME)

clean:
	rm -f $(OBJECTS) $(EXEC_NAME)
