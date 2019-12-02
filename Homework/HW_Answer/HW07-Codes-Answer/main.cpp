#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main() {
    string exp;
    string::iterator it;
    stack<char> s;
    while (1) {
        cout << "Input exp(input 'q' to terminate): ";
        cin >> exp;
        if (exp.compare("q") == 0) {
            cout << ">> Terminate input." << endl;
            break;
        }
        // ( ) + - * /
        for (it = exp.begin(); it != exp.end(); ++it) {
            if (*it == '(') {
                s.push(*it);
            } else if (*it == ')') {
                while (s.top() != '(') {
                    cout << s.top();
                    s.pop();
                }
                s.pop();
            } else if (*it == '+' || *it == '-') {
                if (s.empty()) {
                    s.push(*it);
                } else {
                    if (s.top() == '(') {
                        s.push(*it);
                    } else {
                        cout << s.top();
                        s.pop();
                        s.push(*it);
                    }
                }
            } else if (*it == '*' || *it == '/') {
                if (s.empty()) {
                    s.push(*it);
                } else {
                    if (s.top() == '*' || s.top() == '/') {
                        cout << s.top();
                        s.pop();
                        s.push(*it);
                    } else {
                        s.push(*it);
                    }
                }
            } else {
                cout << *it;
            }
        }
        while (!s.empty()) {
            cout << s.top();
            s.pop();
        }
        cout << endl;
    }
    return 0;
}
