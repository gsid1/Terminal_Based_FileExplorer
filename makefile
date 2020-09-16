CC = g++
CFLAGS = -Wall -std=c++14 -g
DEPS = myheader.h global.h
OBJ = global.o list.o copyfile.o move.o rename.o create.o command.o search.o snapshot.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

term: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	
.PHONY: clean

clean:
	rm -rf *.o term
