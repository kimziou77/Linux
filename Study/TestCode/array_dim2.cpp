#include <array>
#include <iostream>

using namespace std;
int main(int argc, char const *argv[]) {
    array<array<int, 8>, 4> arr = {0};
    array<array<int, 8>, 4>::iterator row;
    array<int, 8>::iterator col;

    cout << "elements of arr" << endl;
    for (row = arr.begin(); row != arr.end(); row++) {
        for (col = (*row).begin(); col != (*row).end(); col++) {
            cout << *col << " ";
        }
        cout << endl;
    }
    cout << endl;

    int i = 1;
    for (row = arr.begin(); row != arr.end(); row++) {
        (*row).fill(i++);
    }
    cout << "after fill" << endl;
    for (row = arr.begin(); row != arr.end(); row++) {
        for (col = (*row).begin(); col != (*row).end(); col++) {
            cout << *col << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "value of arr[0][2] : ";
    cout << (arr.front())[2] << endl;

    cout << "value of arr[1][3] : ";
    cout << (arr.at(1))[3] << endl;

    cout << "value of arr[2][2] : ";
    cout << arr[2][2] << endl;

    return 0;
}
