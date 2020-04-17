all: main.out

main.out : Thread.c Scheduler.c main.c
	gcc main.c Thread.c Scheduler.c -o main.out

Tread.o: Thread.c Thread.h
	gcc Thread.c -o Thread.o

Scheduler.o : Scheduler.c Scheduler.h
	gcc Scheduler.c -o Scheduler.o

clean:
	rm *.out *.o
