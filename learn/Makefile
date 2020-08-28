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
#跟uboot主Makefile分析相关的测试

HOSTARCH := $(shell uname -m | \
	sed -e s/i.86/i386/ \
	    -e s/sun4u/sparc64/ \
	    -e s/arm.*/arm/ \
	    -e s/sa110/arm/ \
	    -e s/powerpc/ppc/ \
	    -e s/ppc64/ppc/ \
	    -e s/macppc/ppc/)

HOSTOS := $(shell uname -s | tr '[:upper:]' '[:lower:]' | \
	    sed -e 's/\(cygwin\).*/cygwin/')



hello:

	@echo $(HOSTARCH)
	@echo $(HOSTOS)


#测试结束	
.PHONY:install clean

install:
	./test

clean:
	-rm -rf *.o test



