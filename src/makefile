CC = gcc
#CFLAGS = -Wall -O2
CFLAGS = -I../include `pkg-config --libs --cflags gtk+-2.0`
LFLAGS = -lpthread

SRCS = main.c login_win.c swing.c net.c handle.c data_show.c common_win.c

OBJS = $(SRCS:.c=.o)

EXEC = type_assis

all:$(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LFLAGS) $(CFLAGS)

clean:
	rm -rf $(OBJS)
