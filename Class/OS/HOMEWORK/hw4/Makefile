all: main.out f

# main.out : Headers.h Init.h MsgQueue.h ReadyQueue.h Scheduler.h Thread.h TestCase1.h
# 	gcc -o main.out main.c MsgQueue.c ReadyQueue.c Scheduler.c Thread.c Init.c TestCase1.c

main.out : Thread.o Scheduler.o main.o MsgQueue.o ReadyQueue.o Init.o Headers.o TestCase1.h TestCase2.h TestCase3.h TestCase4.h
	gcc -g -o main.out main.c Thread.o Scheduler.o MsgQueue.o ReadyQueue.o Init.o Headers.o TestCase1.c TestCase2.c TestCase3.c TestCase4.c

MsgQueue.o : MsgQueue.c MsgQueue.h Headers.h
	gcc -g -c -o MsgQueue.o MsgQueue.c

Init.o : Init.c Headers.h
	gcc -g -c -o Init.o Init.c

Scheduler.o : Scheduler.c Headers.h
	gcc -g -c -o Scheduler.o Scheduler.c

Tread.o: Thread.c Headers.h
	gcc -g -c -o Thread.o Thread.c 

ReadyQueue.o : ReadyQueue.c Headers.h
	gcc -g -c -o ReadyQueue.o ReadyQueue.c

Headers.o : Headers.c Headers.h
	gcc -g -c -o Headers.o Headers.c

f : test.c
	gcc -o f test.c

clean:
	rm *.out *.o
