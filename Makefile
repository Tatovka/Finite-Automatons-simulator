.PHONY: all clean
CC=g++
CFLAGS= -std=c++23 -Wall
INCLUDE=-Iinclude
EXECUTABLE=NFA

SRCS=$(wildcard src/*.cpp)
INCS=$(wildcard include/*.hpp)
OBJS=$(patsubst src/%.cpp,obj/%.o,$(SRCS))
all: $(EXECUTABLE)
obj:
	mkdir obj
obj/%.o: src/%.cpp $(INCS) | obj
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
$(EXECUTABLE): $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) 
clean:
	rm -rf obj
	rm $(EXECUTABLE)
run: clean all
	./$(EXECUTABLE)
