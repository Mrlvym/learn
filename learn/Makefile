OBJS:=test.o
CFLAGS:=-Wall

CC:=gcc
A:=haha
export A

test:$(OBJS)
	
	$(CC) -o test $< -lm

$(OBJS):%.o:%.c

	$(CC) -c $(CFLAGS) $< -o $@

include f.mk

.PHONY:install clean

install:
	./test

clean:
	-rm -rf *.o test



