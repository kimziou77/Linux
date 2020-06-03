all: main.out

main.out : main.o fs_internal.o Headers.o disk.o
	gcc -o main.out main.o fs_internal.o Headers.o disk.o 

main.o : main.c Headers.h 
	gcc -c -o main.o main.c

Headers.o : Headers.c Headers.h
	gcc -c -o Headers.o Headers.c

fs_internal.o : fs_internal.c Headers.h
	gcc -c -o fs_internal.o fs_internal.c

# fs.o : fs.c Headers.h
# 	gcc -c -o fs.o fs.c

disk.o : disk.c disk.h
	gcc -c -o disk.o disk.c

# TestCase1.o : TestCase1.c TestCase1.h
# 	gcc -c -o TestCase1.o TestCase1.c

clean:
	rm *.out *.o MY_DISK
