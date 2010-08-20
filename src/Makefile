CC=gcc
CFLAGS+=-O2 -Wall -s
OBJS=netdt.o arp.o iface.o param.o network.o

all: $(OBJS)
	$(CC) -I./includes $(CFLAGS) -o netdt $(OBJS)
netdt.o: netdt.c ./includes/netdt.h ./includes/defs.h
	$(CC) -I./includes $(CFLAGS) -c -o netdt.o netdt.c
arp.o: arp.c ./includes/arp.h ./includes/defs.h
	$(CC) -I./includes $(CFLAGS) -c -o arp.o arp.c
iface.o: iface.c ./includes/iface.h
	$(CC) -I./includes $(CFLAGS) -c -o iface.o iface.c
param.o: param.c ./includes/param.h
	$(CC) -I./includes $(CFLAGS) -c -o param.o param.c
network.o: network.c ./includes/network.h
	$(CC) -I./includes $(CFLAGS) -c -o network.o network.c
clean:
	rm -rf netdt $(OBJS)
