#
# Tachyon Fighters Makefile
# $Id: Makefile,v 1.1.1.1 2003/08/30 23:36:29 trigen Exp $
# =========================================================

CC = gcc
CFLAGS = -O2 -g -Wall -funsigned-char
LFLAGS = -lm
#CFLAGS = -include /usr/include/mpatrol.h -fno-inline -g
#LFLAGS = -lm -lmpatrol -lelf -lbfd

OBJ = server.o bullet.o collision.o console.o findpath.o \
      genproc.o map.o net.o packet.o send.o player.o ship.o \
      ../common/proto.c

all:	server

clean:
	rm -f server *.o

server.o:	server.c $(INCLUDES)
	$(CC) $(CFLAGS) -c server.c	
bullet.o:	bullet.c $(INCLUDES)
	$(CC) $(CFLAGS) -c bullet.c
collision.o:	collision.c $(INCLUDES)
	$(CC) $(CFLAGS) -c collision.c
console.o:	console.c $(INCLUDES)
	$(CC) $(CFLAGS) -c console.c
findpath.o:	findpath.c $(INCLUDES)
	$(CC) $(CFLAGS) -c findpath.c
genproc.o:	genproc.c $(INCLUDES)
	$(CC) $(CFLAGS) -c genproc.c
map.o:	map.c $(INCLUDES)
	$(CC) $(CFLAGS) -c map.c
net.o:	net.c $(INCLUDES)
	$(CC) $(CFLAGS) -c net.c
packet.o:	packet.c $(INCLUDES)
	$(CC) $(CFLAGS) -c packet.c
send.o:	send.c $(INCLUDES)
	$(CC) $(CFLAGS) -c send.c
player.o:	player.c $(INCLUDES)
	$(CC) $(CFLAGS) -c player.c
ship.o:	ship.c $(INCLUDES)
	$(CC) $(CFLAGS) -c ship.c

server:	$(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ) $(LFLAGS)
