# all: main.out

# main.out : Thread.c Scheduler.c Tmain.c ReadyQueue.c Init.c TestCase1.c TestCase2.c Headers.h TestCase1.h TestCase2.h Init.h ReadyQueue.h Scheduler.h Thread.h
# 	gcc -g -o main.out Tmain.c Thread.c Scheduler.c ReadyQueue.c Init.c TestCase1.c TestCase2.c

# clean:
# 	rm *.out

all: main.out

main.out : Thread.o Scheduler.o main.o ReadyQueue.o Init.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o
	gcc -g -o main.out main.o Thread.o Scheduler.o ReadyQueue.o Init.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o

main.o : main.c Headers.h TestCase1.h TestCase2.h TestCase3.h TestCase4.h
	gcc -g -c -o main.o main.c

TestCase1.o : TestCase1.c TestCase1.h
	gcc -g -c -o TestCase1.o TestCase1.c

TestCase2.o : TestCase2.c TestCase2.h
	gcc -g -c -o TestCase2.o TestCase2.c

TestCase3.o : TestCase3.c TestCase3.h
	gcc -g -c -o TestCase3.o TestCase3.c

TestCase4.o : TestCase4.c TestCase4.h
	gcc -g -c -o TestCase4.o TestCase4.c

Init.o : Init.c Headers.h
	gcc -g -c -o Init.o Init.c

Scheduler.o : Scheduler.c Headers.h
	gcc -g -c -o Scheduler.o Scheduler.c

Tread.o: Thread.c Headers.h
	gcc -g -c -o Thread.o Thread.c 

ReadyQueue.o : ReadyQueue.c Headers.h
	gcc -g -c -o ReadyQueue.o ReadyQueue.c

clean:
	rm *.out *.o
