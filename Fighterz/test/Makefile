#
# Tachyon Fighters Makefile
# $Id: Makefile,v 1.1 2003/08/30 23:36:29 trigen Exp $
# =========================================================

CC = gcc
CFLAGS = -O2 -g -Wall -I../server
LFLAGS = -lm
#CFLAGS = -include /usr/include/mpatrol.h -fno-inline -g
#LFLAGS = -lm -lmpatrol -lelf -lbfd

OBJ = packet.c net.c main.c ../common/proto.c send.c

all:	test

clean:
	rm -f test *.o

test.o:	test.c $(INCLUDES)
	$(CC) $(CFLAGS) -c test.c	
packet.o:	packet.c $(INCLUDES)
	$(CC) $(CFLAGS) -c packet.c	
net.o:	net.c $(INCLUDES)
	$(CC) $(CFLAGS) -c net.c	

test:	$(OBJ)
	$(CC) $(CFLAGS) -o test $(OBJ) $(LFLAGS)
