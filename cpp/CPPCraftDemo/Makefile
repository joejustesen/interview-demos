# the compiler: gcc for C program, define as g++ for C++
CC = g++

CPPFLAGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O3 -std=gnu++2a
DEBUG_CPPFLAGS = -g -Wall -Wextra -Wpedantic -std=gnu++2a

  # the build target executable:
TARGET = demo
FILES = CPPCraftDemo.cpp TrieIndex.cpp

all: $(TARGET)

debug: $(FILES)
	$(CC) $(DEBUG_CPPFLAGS) -o $(TARGET) $(FILES)

$(TARGET): $(FILES)
	$(CC) $(CPPFLAGS) -o $(TARGET) $(FILES)

clean:
	$(RM) $(TARGET)

StopWatch.o: StopWatch.cpp StopWatch.h
TrieIndex.o: TrieIndex.cpp TrieIndex.h
CPPCraftDemo.o: CPPCraftDemo.cpp StopWatch.h TrieIndex.h