all: main.out testcase.out f

main.out : Headers.h fs.h disk.h main.c Headers.c fs.c disk.c fs_internal.c
	gcc -o main.out main.c Headers.c disk.c fs.c fs_internal.c

testcase.out : Headers.h fs.h disk.h testcase.c Headers.c fs.c disk.c fs_internal.c
	gcc -o testcase.out testcase.c Headers.c disk.c fs.c fs_internal.c

f : test.c
	gcc -o f test.c

clean:
	rm *.out
