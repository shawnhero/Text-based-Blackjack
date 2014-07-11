CC=clang++
CFLAGS  += -O3
CFLAGS  += -std=c++0x
CFLAGS  += -g -c -Wall
#-pg -D_DEBUG
LDFLAGS=
SOURCES=main.cpp cards.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bj

all: preclean $(SOURCES) $(EXECUTABLE) clean
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

preclean:
	#rm *o
clean:
	rm $(OBJECTS)