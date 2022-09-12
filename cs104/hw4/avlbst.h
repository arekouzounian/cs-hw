#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    signed char getBalance() const;
    void setBalance(signed char balance);
    void updateBalance(signed char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    signed char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
signed char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(signed char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(signed char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value>& new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    // Add helper functions here
    void leftRotate(AVLNode<Key, Value>* n);
    void rightRotate(AVLNode<Key, Value>* n);

    bool isLeftChild(Node<Key, Value>* n);
    bool isRightChild(Node<Key, Value>* n);

    void insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    void removeFix(AVLNode<Key, Value>* n, signed char diff);
};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item)
{
    // TODO
    if (!this->root_)
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    Node<Key, Value>* n = this->root_;
    Node<Key, Value>* prev;
    while (n)
    {
        prev = n;
        if (new_item.first < n->getKey())
        {
            n = n->getLeft();
        }
        else if (new_item.first > n->getKey())
        {
            n = n->getRight();
        }
        else
        {
            n->setValue(new_item.second);
            return; //can I just return here? 
        }
    }

    AVLNode<Key, Value>* p = static_cast<AVLNode<Key, Value>*>(prev);

    n = new AVLNode<Key, Value>(new_item.first, new_item.second, p);
    AVLNode<Key, Value>* n_cast = static_cast<AVLNode<Key, Value>*>(n);
    if (n->getKey() < p->getKey())
    {
        p->setLeft(n);
    }
    else
    {
        p->setRight(n);
    }

    if (p->getBalance() == -1 || p->getBalance() == 1)
    {
        p->setBalance(0);
        return;
    }
    else
    {
        if (isLeftChild(n))
        {
            p->updateBalance(-1);
        }
        else
        {
            p->updateBalance(1);
        }
        insertFix(n_cast, p);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p)
{
    if (!n || !p) return;
    AVLNode<Key, Value>* g = p->getParent();

    if (!g) return;

    if (isLeftChild(p))
    {
        g->updateBalance(-1);
    }
    else
    {
        g->updateBalance(1);
    }

    switch (g->getBalance())
    {
    case 0:
        return;
    case -1:
    case 1:
        insertFix(p, g);
        break;
    case -2:
    {
        //if left left, rotate right rooted at p 
        if (isLeftChild(n))
        {
            rightRotate(p);
            g->setBalance(0);
            p->setBalance(0);
        }
        //if left right, rotate left rooted at n, rotate right rooted at g 
        else
        {
            leftRotate(n);
            rightRotate(n);

            if (n->getBalance() == -1)
            {
                p->setBalance(0);
                g->setBalance(1);
            }
            else if (n->getBalance() == 0)
            {
                p->setBalance(0);
                g->setBalance(0);
            }
            else
            {
                p->setBalance(-1);
                g->setBalance(0);
            }
            n->setBalance(0);
        }
    }
    break;
    case 2:
    {
        //if right right, rotate left rooted at g
        if (isRightChild(n))
        {
            leftRotate(p);
            p->setBalance(0);
            g->setBalance(0);
        }
        //if right left, rotate right rooted at n
        else
        {
            rightRotate(n);
            leftRotate(n);

            if (n->getBalance() == -1)
            {
                p->setBalance(1);
                g->setBalance(0);
            }
            else if (n->getBalance() == 0)
            {
                p->setBalance(0);
                g->setBalance(0);
            }
            else
            {
                p->setBalance(0);
                g->setBalance(-1);
            }
            n->setBalance(0);
        }
    }
    break;
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    Node<Key, Value>* n = this->internalFind(key);
    if (!n) return;

    if (n->getLeft() && n->getRight())
    {
        nodeSwap(static_cast< AVLNode<Key, Value>* >(n), static_cast< AVLNode<Key, Value>* >(BinarySearchTree<Key, Value>::successor(n)));
    }

    signed char diff = 0;
    AVLNode<Key, Value>* p = static_cast<AVLNode<Key, Value>*>(n->getParent());
    if (p)
    {
        if (isLeftChild(n))
        {
            diff = 1;
        }
        else
        {
            diff = -1;
        }
    }

    if (n->getLeft()) //left only
    {
        if (n == this->root_)
        {
            this->root_ = n->getLeft();
        }
        else
        {
            if (isLeftChild(n))
            {
                p->setLeft(n->getLeft());
            }
            else
            {
                p->setRight(n->getLeft());
            }
        }
        n->getLeft()->setParent(p);
    }
    else if (n->getRight()) //right only
    {
        if (n == this->root_)
        {
            this->root_ = n->getRight();
        }
        else
        {
            if (isLeftChild(n))
            {
                p->setLeft(n->getRight());
            }
            else
            {
                p->setRight(n->getRight());
            }
        }
        n->getRight()->setParent(p);
    }
    else //leaf
    {
        if (n == this->root_)
        {
            this->root_ = nullptr;
        }
        else
        {
            if (isLeftChild(n))
            {
                p->setLeft(nullptr);
            }
            else
            {
                p->setRight(nullptr);
            }
        }
    }
    delete n;

    removeFix(p, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, signed char diff)
{
    if (!n) return;

    AVLNode<Key, Value>* p = n->getParent();
    signed char ndiff = -1;
    if (p && isLeftChild(n))
    {
        ndiff = 1;
    }

    switch (n->getBalance() + diff)
    {
    case -2:
    {
        AVLNode<Key, Value>* c = n->getLeft();
        if (c->getBalance() == -1) {
        rightRotate(c);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
                }
        else if (c->getBalance() == 1)
        {
            AVLNode<Key, Value>* g = c->getRight();
            int b = g->getBalance();
            leftRotate(g);
            rightRotate(g);

            if (b == 1)
            {
                n->setBalance(0);
                c->setBalance(-1);
            }
            else if (b == -1)
            {
                n->setBalance(1);
                c->setBalance(0);
            }
            else
            {
                n->setBalance(0);
                c->setBalance(0);
            }
            g->setBalance(0);
        }
        else
        {
            rightRotate(c);
            n->setBalance(-1);
            c->setBalance(1);
        }
        removeFix(p, ndiff);
    }
    case -1:
    {
        n->setBalance(-1);
    }
    break;
    case 0:
    {
        n->setBalance(0);
        removeFix(p, ndiff);
    }
    break;
    case 1:
    {
        n->setBalance(1);
    }
    break;
    case 2:
    {
        AVLNode<Key, Value>* c = n->getRight();
        if (c->getBalance() == 1) {
        leftRotate(c);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
                }
        else if (c->getBalance() == -1)
        {
            AVLNode<Key, Value>* g = c->getLeft();
            int b = g->getBalance();
            rightRotate(g);
            leftRotate(g);

            if (b == -1)
            {
                n->setBalance(0);
                c->setBalance(1);
            }
            else if (b == 1)
            {
                n->setBalance(-1);
                c->setBalance(0);
            }
            else
            {
                n->setBalance(0);
                c->setBalance(0);
            }
            g->setBalance(0);
        }
        else
        {
            leftRotate(c);
            n->setBalance(1);
            c->setBalance(-1);
        }
    }
    break;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    signed char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template <class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* n)
{
    if (!n->getParent()) return;
    if (!isRightChild(n)) return;

    AVLNode<Key, Value>* parent = n->getParent()->getParent();
    AVLNode<Key, Value>* newLeft = n->getParent();
    AVLNode<Key, Value>* oldLeft = n->getLeft();

    n->setLeft(newLeft);
    n->setParent(parent);
    if (parent)
    {
        if (parent->getLeft() == newLeft)
        {
            parent->setLeft(n);
        }
        else
        {
            parent->setRight(n);
        }
    }
    else
    {
        this->root_ = n;
    }
    newLeft->setParent(n);
    newLeft->setRight(oldLeft);
    if (oldLeft)
        oldLeft->setParent(newLeft);
}

template <class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* n)
{
    if (!n->getParent()) return;
    if (!isLeftChild(n)) return;

    AVLNode<Key, Value>* parent = n->getParent()->getParent();
    AVLNode<Key, Value>* newRight = n->getParent();
    AVLNode<Key, Value>* oldRight = n->getRight();

    n->setRight(newRight);
    n->setParent(parent);
    if (parent)
    {
        if (parent->getLeft() == newRight)
        {
            parent->setLeft(n);
        }
        else
        {
            parent->setRight(n);
        }
    }
    else
    {
        this->root_ = n;
    }
    newRight->setParent(n);
    newRight->setLeft(oldRight);
    if (oldRight)
        oldRight->setParent(newRight);
}

template <class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(Node<Key, Value>* n)
{
    return n->getParent()->getLeft() == n;
}

template <class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(Node<Key, Value>* n)
{
    return n->getParent()->getRight() == n;
}

#endif
