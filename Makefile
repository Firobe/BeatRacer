COMPILER = g++
CFLAGS = -Wall
LIBS = 
SOURCES = $(wildcard src/*.cpp)
EXEC_NAME = btrc
OBJECTS = $(SOURCES:.cpp=.o)
.PRECIOUS : %.o
.PHONY : clean
.PHONY : run

all: $(OBJECTS)
	$(COMPILER) -o $(EXEC_NAME) $(OBJECTS) $(LIBS)

%.o : %.c
	$(COMPILER) -c $(CFLAGS) $< -o $@

run: all
	./$(EXEC_NAME)

clean:
	rm -f $(OBJECTS) $(EXEC_NAME)
