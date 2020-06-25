all: testcase.out f

testcase.out : Headers.h fs.h disk.h testcase.c Headers.c fs.c disk.c fs_internal.c
	gcc -o testcase.out testcase.c Headers.c disk.c fs.c fs_internal.c

f : test.c
	gcc -o f test.c

clean:
	rm *.out MY_DISK
