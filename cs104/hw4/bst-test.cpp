#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    /*
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');
    */
    // AVL Tree Tests
    AVLTree<int,int> at;
    at.insert(std::make_pair(50, 1));
    at.insert(std::make_pair(32, 1));
    at.insert(std::make_pair(75, 1));
    at.insert(std::make_pair(21, 1));
    at.insert(std::make_pair(44, 1));
    at.insert(std::make_pair(61, 1));
    at.insert(std::make_pair(35, 1));
    at.insert(std::make_pair(47, 1));

    cout << "\nAVLTree contents:" << endl;
    at.print();

    cout << "After Removal:" << endl;
    at.remove(50);
    at.print(); 
    
    string a; 
    cin >> a; 

    return 0;
}
