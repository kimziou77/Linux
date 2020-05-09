
all: main.out

main.out : Thread.o Scheduler.o main.o ReadyQueue.o TestCase1.o
	gcc -g -o main.out main.o Thread.o Scheduler.o ReadyQueue.o TestCase1.o

main.o : test.c Headers.h TestCase1.h
	gcc -g -c -o main.o test.c

TestCase1.o : TestCase1.c TestCase1.h
	gcc -g -c -o TestCase1.o TestCase1.c


Scheduler.o : Scheduler.c Headers.h
	gcc -g -c -o Scheduler.o Scheduler.c

Tread.o: Thread.c Headers.h
	gcc -g -c -o Thread.o Thread.c 

ReadyQueue.o : ReadyQueue.c Headers.h
	gcc -g -c -o ReadyQueue.o ReadyQueue.c

clean:
	rm *.out *.o
