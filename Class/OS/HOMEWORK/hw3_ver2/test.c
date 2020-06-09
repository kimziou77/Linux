#include <stdlib.h>

int main(void){
    
    // system("rm MY_DISK");
    system("make clean");
    system("make");
    system("clear");
    // system("./main.out");
    
    // system("./testcase.out format 1");
    system("./testcase.out readwrite 2");
    // system("./testcase.out readwrite 3");
    // system("./testcase.out format 4");
    return 0;
}
