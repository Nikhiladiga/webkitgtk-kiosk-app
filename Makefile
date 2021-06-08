PROGRAM=kbrowser
OBJECTS=kbrowser.o

CC=g++
CFLAGS=$(shell pkg-config --cflags gtkmm-3.0 webkit2gtk-4.0)
CFLAGS+= -Wall -g -std=c++11

LDFLAGS=$(shell pkg-config --libs gtkmm-3.0 webkit2gtk-4.0)


kbrowser: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(PROGRAM) 

%.o : %.cc
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -rf $(OBJECTS) $(PROGRAM) 