#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool pathDepth(Node* root, int depth, int& leafDepth);


bool equalPaths(Node * root)
{
    // Add your code below

    if(root == nullptr) return true;        //tree is empty, all paths are equal automatically

    int leafDepth = -9;             
    return pathDepth(root, 0, leafDepth);

}

//need to keep track of the depth with this one
bool pathDepth(Node* root, int depth, int& leafDepth){      //needed to check against depth, so added leafDepth ref to keep track for all calls

    if(root->left == nullptr && root->right == nullptr){        //checking for no children (leaf)
        
        if(leafDepth == -9){            //true if we haven't ran into a leaf yet
            leafDepth = depth;          //stores the depth to check against other leafs
            return true;                //since this was the first leaf no checking against needed - return true
        }
        
        if(depth == leafDepth){     //when checking other leafs compare the depth they were found with first leaf depth 
            return true;
        }else{
            return false;
        }

        
    }
    //bool variables to save the status of both left and right subtrees
    bool lsubTree = true;    
    bool rsubTree = true;

    if(root->left != nullptr) lsubTree = pathDepth(root->left, depth + 1, leafDepth);      //recursive call, checks left subtree with updated root, depth, and keeps ref to original leaf to check
    if(root->right != nullptr) rsubTree = pathDepth(root->right, depth + 1, leafDepth);     //same for right check after left

    if(lsubTree && rsubTree){       //if both the left AND right subtrees are true, then the equal paths condition is upheld - return true
        return true;
    }else{
        return false;
    }


}