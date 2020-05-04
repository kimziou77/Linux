all: main.out

main.out : Thread.o Scheduler.o main.o ReadyQueue.o Init.o
	gcc -o main.out main.o Thread.o Scheduler.o ReadyQueue.o Init.o

main.o : main.c Headers.h
	gcc -c -o main.o main.c

Init.o : Init.c Headers.h
	gcc -c -o Init.o Init.c	

Scheduler.o : Scheduler.c Headers.h
	gcc -c -o Scheduler.o Scheduler.c

Tread.o: Thread.c Headers.h
	gcc -c -o Thread.o Thread.c 

ReadyQueue.o : ReadyQueue.c Headers.h
	gcc -c -o ReadyQueue.o ReadyQueue.c

clean:
	rm *.out *.o
