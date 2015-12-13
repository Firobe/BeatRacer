COMPILER = g++
ADD_CFLAGS = -Wall
LIBS = glfw3 glu
CXXFLAGS = `pkg-config --static --cflags $(LIBS)` -Wall
LINKS = `pkg-config --static --libs $(LIBS)`
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
