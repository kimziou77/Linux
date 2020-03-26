#include <array>
#include <iostream>

using namespace std;
int main(int argc, char const *argv[]) {
    array<int, 8> arr1;
    array<int, 8> arr2 = {0};
    array<int, 8> arr3 = {1, 2, 3, 4};
    array<int, 8> arr4 = {1, 2, 3, 4, 5, 6, 7, 8};

    cout << "elements of arr1: ";
    array<int, 8>::iterator iter;
    for (iter = arr1.begin(); iter != arr1.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    cout << "elements of arr2: ";
    for (size_t i = 0; i < arr2.size(); i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    cout << "elements of arr3: ";
    for (size_t i = 0; i < arr3.size(); i++) {
        cout << arr3.at(i) << " ";
    }
    cout << endl;

    cout << "elements of arr4 (reverse): ";
    array<int, 8>::reverse_iterator riter;
    for (riter = arr4.rbegin(); riter != arr4.rend(); ++riter) {
        cout << *riter << " ";
    }
    cout << endl;

    return 0;
}
