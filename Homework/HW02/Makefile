all: mywrite.out myread.out

mywrite.out: mywrite.c MyStudent.h
	gcc mywrite.c -o mywrite.out

myread.out: myread.c MyStudent.h
	gcc myread.c -o myread.out

clean:
	rm *.out student.dat
