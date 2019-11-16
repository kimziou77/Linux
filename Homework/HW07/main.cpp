/*
 * Student ID: 2018203062
 * Name: KIMSUBIN
 */
#include <iostream>
#include <stack>
#include <string>
#include <list>

using namespace std;
bool isOperation(char s){
    return s=='+'||s=='-'||s=='*'||s=='/';//연산자 이면
}
int main(void) {

    while(1){
        stack<char> numbers;
        stack<char> operations;
        list<char> result;
        string s;
        char tmp;
        cout<<"Input exp(input 'q' to terminate): ";   cin>>s;
        if(s[0]=='q') {
            cout<<">> Terminate Input."<<endl;
            return 0;
        }
        for(int i=0;i<s.size();i++){
            if(isOperation(s[i])){//연산자 이면
                //operation
                if (!operations.empty()) {
                    char ot=operations.top();
                    while(operations.size()>0&&isOperation(ot)&&(s[i]=='+'||s[i]=='-')) {
                        //+,- 이면서 *와 /가 들어있는동안 pop해줘야 한다.
                        //*와 /가 들어있는동안 pop해줘야 한다.
                        tmp = operations.top();
                        result.push_back(tmp);
                        operations.pop();
                    }
                }
                //우선순위에 따라서 pop하고
                operations.push(s[i]);//자기는 들어간다
            }
            else if(s[i]=='('){
                operations.push(s[i]);
            }
            else if(s[i]==')'){
                //여는 괄호가 나올때까지 pop을 하고 출력한다.
                while (operations.top()!='(') {
                    tmp = operations.top();
                    result.push_back(tmp);
                    //여는괄호가 아닌동안 push_back()
                    operations.pop();
                }
                //여는괄호이면
                operations.pop();
            }
            else{
                result.push_back(s[i]);
            }
        }
        cout<<"Postfix notation : ";

        while(!operations.empty()) {
            tmp = operations.top();
            result.push_back(tmp);
            operations.pop();
        }
        for (auto i : result) {
            cout << i << " ";
        }
        cout << endl;

    }

    return 0;
}
