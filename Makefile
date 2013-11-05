#g++ -std=c++11 test.cc -I ~/SFML-2.1/include -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -L ~/SFML-2.1/lib -Wl,-rpath=~/SFML-2.1/lib -Wl,-rpath-link=~/SFML-2.1/lib -Wall
SFML=/home/jaine/Downloads/SFML-2.1
CC=g++
CFLAGS=-c -g -std=c++0x -Wall -Wextra -pedantic -I $(SFML)/include
LDFLAGS=-lBox2D -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L $(SFML)/lib -Wl,-rpath=$(SFML)/lib -Wl,-rpath-link=$(SFML)/lib
SOURCES=graphicstestmain.cc
OBJECTS=graphicstestmain.o
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
