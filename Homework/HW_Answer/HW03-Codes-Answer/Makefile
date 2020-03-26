CFLAGS = -std=c11

all: myls.out

debug: CFLAGS += -g
debug: myls

myls.out:
	gcc $(CFLAGS) myls.c -o myls.out
