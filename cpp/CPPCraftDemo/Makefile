# the compiler: gcc for C program, define as g++ for C++
CC = g++

CPPFLAGS = -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O3 -std=gnu++2a
DEBUG_CPPFLAGS = -g -Wall -Wextra -Wpedantic -std=gnu++2a

  # the build target executable:
TARGET = demo
FILES = CPPCraftDemo.cpp TrieIndex.cpp
HEADERS = TrieIndex.h StopWatch.h

all: $(TARGET)

debug: $(FILES) $(HEADERS)
	$(CC) $(DEBUG_CPPFLAGS) -o $(TARGET) $(FILES)

$(TARGET): $(FILES) $(HEADERS)
	$(CC) $(CPPFLAGS) -o $(TARGET) $(FILES)

clean:
	$(RM) $(TARGET)
