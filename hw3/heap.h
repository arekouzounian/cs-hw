#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
    /**
     * @brief Construct a new Heap object
     *
     * @param m ary-ness of heap tree (default to 2)
     * @param c binary predicate function/functor that takes two items
     *          as an argument and returns a bool if the first argument has
     *          priority over the second.
     */
    Heap(int m = 2, PComparator c = PComparator());

    /**
    * @brief Destroy the Heap object
    *
    */
    ~Heap();

    /**
     * @brief Push an item to the heap
     *
     * @param item item to heap
     */
    void push(const T& item);

    /**
     * @brief Returns the top (priority) item
     *
     * @return T const& top priority item
     * @throw std::underflow_error if the heap is empty
     */
    T const& top() const;

    /**
     * @brief Remove the top priority item
     *
     * @throw std::underflow_error if the heap is empty
     */
    void pop();

    /// returns true if the heap is empty

    /**
     * @brief Returns true if the heap is empty
     *
     */
    bool empty() const;

private:
    /// Add whatever helper functions and data members you need below

    std::vector<T> items;
    int m;
    PComparator comp;

    void HeapifyDown(int index);
    void HeapifyUp(int index);

    int getParent(const int& index);
    int getBestChild(const int& index);
};

// Add implementation of member functions here

template <typename T, typename PComparator>
Heap<T, PComparator>::Heap(int m, PComparator c)
    : m(m), comp(c)
{
    items = std::vector<T>();
}

template <typename T, typename PComparator>
Heap<T, PComparator>::~Heap() {}

// We will start top() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
T const& Heap<T, PComparator>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if (empty())
    {
        // ================================
        // throw the appropriate exception
        // ================================
        throw std::underflow_error("Heap is empty!");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return items[0];
}


// We will start pop() for you to handle the case of 
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T, PComparator>::pop()
{
    if (empty())
    {
        // ================================
        // throw the appropriate exception
        // ================================

        throw std::underflow_error("Heap is empty!");
    }

    std::swap(items[0], items[items.size() - 1]);
    items.pop_back();
    HeapifyDown(0);
}

template <typename T, typename PComparator>
bool Heap<T, PComparator>::empty() const
{
    return this->items.empty();
}


template <typename T, typename PComparator>
void Heap<T, PComparator>::push(const T& item)
{
    items.push_back(item);
    HeapifyUp(items.size() - 1);
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::HeapifyUp(int index)
{
    int parInd = getParent(index);
    while (parInd != -1 && comp(items[index], items[parInd]))
    {
        std::swap(items[index], items[parInd]);
        index = parInd;
        parInd = getParent(parInd);
    }
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::HeapifyDown(int index)
{
    int best_child = getBestChild(index);
    while (best_child != -1 && !comp(items[index], items[best_child]))
    {
        std::swap(items[index], items[best_child]);
        index = best_child;
        best_child = getBestChild(index);
    }
}

template <typename T, typename PComparator>
int Heap<T, PComparator>::getBestChild(const int& index)
{
    int ret = (index * m) + 1;
    if(ret >= items.size())
        return -1; 
    
    for(int i = 2; i <= m && ((index * m) + i) < items.size(); ++i)
    {
        if(comp(items[(index * m) + i], items[ret]))
            ret = (index * m) + i;
    }

    return ret;
}


template <typename T, typename PComparator>
int Heap<T, PComparator>::getParent(const int& index)
{
    if (index >= items.size() || index <= 0)
        return -1;
    return int((index - 1) / m);
}


#endif