CFLAGS = -std=c11

all: CFLAGS += -O2
all: mywrite.out myread.out

debug: CFLAGS += -g
debug: mywrite.out myread.out

mywrite.out: mywrite.c MyStudent.h
	gcc $(CFLAGS) mywrite.c -o mywrite.out

myread.out: myread.c MyStudent.h
	gcc $(CFLAGS) myread.c -o myread.out

clean:
	rm -rf *.out* *.dat
