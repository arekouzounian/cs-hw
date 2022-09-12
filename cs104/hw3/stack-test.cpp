
#include <iostream>
#include "stack.h"

using namespace std;
int main()
{
    Stack<int> stack = Stack<int>();

    cout << "stack should be empty" << endl;
    cout << "Value of empty: " << stack.empty() << endl;

    stack.push(12);
    cout << stack.top() << endl;

    cout << "stack should have one element" << endl;
    cout << "Value of empty: " << stack.empty() << endl;
    
    stack.pop();

    cout << "stack should be empty" << endl;
    cout << "value of empty: " << stack.empty() << endl;


    return 0;
}