all: main.out

main.out : Thread.o Scheduler.o main.o ReadyQueue.o Init.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o
	gcc -o main.out main.o Thread.o Scheduler.o ReadyQueue.o Init.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o

main.o : Tmain.c Headers.h TestCase1.h
	gcc -c -o main.o Tmain.c

TestCase1.o : TestCase1.c TestCase1.h
	gcc -c -o TestCase1.o TestCase1.c

TestCase2.o : TestCase2.c TestCase2.h
	gcc -c -o TestCase2.o TestCase2.c

TestCase3.o : TestCase3.c TestCase3.h
	gcc -c -o TestCase3.o TestCase3.c

TestCase4.o : TestCase4.c TestCase4.h
	gcc -c -o TestCase4.o TestCase4.c

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
