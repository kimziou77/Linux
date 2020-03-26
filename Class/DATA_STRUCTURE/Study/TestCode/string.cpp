#include <iostream>
#include <string>
#include <vector>

using namespace std;
int main(int argc, char const *argv[]) {
    string str1 = "Hello world";
    cout << "str1 : " << str1 << endl;

    string::reverse_iterator riter;
    cout << "str1(reverse): ";
    for (riter = str1.rbegin(); riter != str1.rend(); ++riter) {
        cout << *riter;
    }
    cout << endl << endl;

    string str2 = "Bonjour!";
    cout << "str2 : " << str2 << endl << endl;
    str1.swap(str2);
    cout << "---After swap---" << endl;
    cout << "str1 : " << str1 << endl;
    cout << "str2 : " << str2 << endl;

    return 0;
}
