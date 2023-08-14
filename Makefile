CC = gcc
CFLAGS = -Wall -Wextra -O0 -g

PROG = parse
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(PROG)