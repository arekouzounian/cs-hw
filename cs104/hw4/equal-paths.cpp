#include <algorithm>
#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int depthChecker(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (!root) return true; 
    //checks for equal max depths of both left and right subtrees
    //if the one subtree contains a node with larger depth than the 
    //other, we know that there cannot be equal paths. 
    if(!root->left && root->right) {
        return equalPaths(root->right);
    }
    else if (root->left && !root->right) {
        return equalPaths(root->left);
    }
    else if (!root->left && !root->right) {
        return true; 
    }

    return depthChecker(root->left) == depthChecker(root->right);
}


int depthChecker(Node* root)
{
    //recursively checks the maximum depth of the given root node 
    //by checking both left and right subtrees
    if (!root) return 0; 
    return std::max(1 + depthChecker(root->left), 1 + depthChecker(root->right));
}