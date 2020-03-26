all: write.out read.out

write.out: MyStudent.cpp main.cpp MyStudent.hpp
	g++ MyStudent.cpp main.cpp -o write.out

read.out: myread.cpp MyStudent.cpp MyStudent.hpp
	g++ myread.cpp MyStudent.cpp -o read.out

clean:
	rm -rf *.out StudentList.dat
