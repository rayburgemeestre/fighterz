CC = gcc
CFLAGS = -O2

OBJ = server.o bullet.o collition.o console.o findpath.o findpath_pq.o genproc.o map.o parse.o player.o ship.o socket.o

all:	server

clean:
	rm -f *.o server

server.o:	server.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c server.cpp	
bullet.o:	bullet.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c bullet.cpp
collition.o:	collition.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c collition.cpp
console.o:	console.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c console.cpp
findpath.o:	findpath.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c findpath.cpp
findpath_pq.o:	findpath_pq.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c findpath_pq.cpp
genproc.o:	genproc.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c genproc.cpp
map.o:	map.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c map.cpp
parse.o:	parse.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c parse.cpp
player.o:	player.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c player.cpp
ship.o:	ship.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c ship.cpp
socket.o:	socket.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c socket.cpp

server:	$(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ) -lm
