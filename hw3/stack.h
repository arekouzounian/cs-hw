#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>

// Use inheritance from std::vector (choose public/private) as appropriate

// Since we want to have the Stack use the members of vector but not give the 
// end users access to the internal vector, we use private inheritance
template <typename T>
class Stack : private std::vector<T>
{
public:
    Stack();
    ~Stack();
    bool empty() const;
    size_t size() const;
    void push(const T& item);
    void pop();  // throws std::underflow_error if empty
    const T& top() const; // throws std::underflow_error if empty
    // Add other members only if necessary
};

template <typename T>
Stack<T>::Stack()
    : std::vector<T>()
{}

template <typename T>
Stack<T>::~Stack()
{}

template <typename T>
bool Stack<T>::empty() const
{
    return std::vector<T>::empty();
}

template <typename T>
size_t Stack<T>::size() const
{
    return std::vector<T>::size();
}

template <typename T>
void Stack<T>::push(const T& item)
{
    std::vector<T>::insert(std::vector<T>::begin(), item);
}

template <typename T>
void Stack<T>::pop() 
{
    if (this->empty())
        throw std::underflow_error("Stack is empty!");

    std::vector<T>::erase(std::vector<T>::begin());
}

template <typename T>
const T& Stack<T>::top() const
{
    if (this->empty())
        throw std::underflow_error("Stack is empty!");

    return std::vector<T>::at(0);
}


#endif