#CC=clang++
CC=g++
CFLAGS  += -O3
CFLAGS  += -std=c++0x
CFLAGS  += -g -c -Wall
#-pg -D_DEBUG
LDFLAGS=
SOURCES=main.cpp game.cpp cards.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bj

all: $(SOURCES) $(EXECUTABLE) clean
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

main.o: game.h cards.h
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)