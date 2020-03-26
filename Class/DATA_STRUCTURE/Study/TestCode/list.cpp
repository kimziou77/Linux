#include <iostream>
#include <list>

using namespace std;
int main(int argc, char const *argv[]) {
    double myDoubles[] = {12.15, 2.72,  73.0,  12.77, 3.14,
                          12.77, 73.35, 72.25, 15.3,  72.25};
    list<double> myList(myDoubles, myDoubles + 10);
    list<double>::iterator iter;
    cout << ">> nodes of myList: ";
    for (iter = myList.begin(); iter != myList.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    cout << "<<myList.push_front(1.4):" << endl;
    myList.push_front(1.4);

    cout << "<<myList.push_back(1.4);" << endl;
    myList.push_back(1.4);

    cout << ">> nodes of myList: ";
    for (iter = myList.begin(); iter != myList.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    cout << "<<myList.sort();" << endl;
    myList.sort();

    cout << ">> nodes of myList: ";
    for (iter = myList.begin(); iter != myList.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    myList.unique();
    cout << ">> nodes of myList: ";
    for (iter = myList.begin(); iter != myList.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    return 0;
}
