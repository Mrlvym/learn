OBJS=*.c
CC=gcc
A=haha
export A

test:$(OBJS)

	$(CC) -o test $(OBJS) -lm

%.o:%.c %.h

#	$(CC) -c *.c

include f.mk

.PHONY:install clean

install:
	./test

clean:
	-rm -rf *.o test



