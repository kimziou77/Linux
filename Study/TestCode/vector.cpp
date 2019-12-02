#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[]) {
    vector<int> v(3);
    vector<int>::iterator iter;
    cout << "size of v: " << v.size() << endl;
    cout << "capacity of v : " << v.capacity() << endl;
    cout << "elements of v: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl << endl;
    for (int i = 0; i < v.size(); i++) {
        v[i] = i + 1;
    }
    for (int i = 10; i < 15; i++) {
        v.push_back(i);
    }
    cout << "---After push_back ---" << endl;
    cout << "size of v" << v.size() << endl;
    cout << "capacity of v : " << v.capacity() << endl;
    cout << "elements of v: ";
    for (iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl << endl;
    v.pop_back();
    v.pop_back();

    cout << "---After pop_back()---" << endl;
    cout << "size of v : " << v.size() << endl;
    cout << "capacity of v : " << v.capacity() << endl;
    cout << "elements of v: ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
    return 0;
}
