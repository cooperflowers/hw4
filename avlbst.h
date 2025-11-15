#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
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
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
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
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key,Value>* p);  
    AVLNode<Key, Value>* rotateRight(AVLNode<Key,Value>* p); 
    int height(AVLNode<Key, Value>* n) const;
    int balance(AVLNode<Key, Value>* n) const;
    AVLNode<Key, Value>* insertHelp(AVLNode<Key, Value>* r, const std::pair<const Key, Value> &new_item);

};

template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* n) const{

    if(n == nullptr){
        return 0;                           //empty, height is 0/ base case
    }

    int leftHeight = height(n->getLeft());              //go thru until null, them pop back up adding 1
    int rightHeight = height(n->getRight());

    return 1 + std::max(leftHeight, rightHeight);       //adds one to the height each recursive call 


}

template<class Key, class Value>
int AVLTree<Key, Value>::balance(AVLNode<Key, Value>* n) const{
    if(n == nullptr){
        return 0;
    }

    int leftHeight = height(n->getLeft());            
    int rightHeight = height(n->getRight());

    return rightHeight - leftHeight;                    //same as height, just return balance instead

}


template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* p){

    if(p == nullptr){               //nothing to rotate
        return p;
    }

    AVLNode<Key, Value>* c = p->getRight();                     //saves right child of p
    if(c == nullptr){                                               //no right child, no rotation
        return p;                                                      
    }

    AVLNode<Key, Value>* temp = c->getLeft();              //c left subtree
   

    c->setLeft(p);                                      //performing rotate
    p->setRight(temp);
    AVLNode<Key, Value>* parent = p->getParent();           //save old parent before updating new
    c->setParent(parent);     
    p->setParent(c);

    if(temp != nullptr){                                //if temp exists update the original c left subtree to attach to p
        temp->setParent(p);
    }
    if(parent == nullptr){                         //if p was the root we need to update c as new root
        this->root_ = c;
    }else if(parent->getLeft() == p){           //the original parent was a left child
        parent->setLeft(c);                     //so c will be a left child
    }else{                                          //parent was right child
        parent->setRight(c);
    }

    
    c->setBalance(static_cast<int8_t>(balance(c)));               //set new balance of child
    p->setBalance(static_cast<int8_t>(balance(p)));                    //set new balance of parent

    return c;

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* p){               //essentially same as left rotate 
    if(p == nullptr){
        return p;
    }

    AVLNode<Key, Value>* c = p->getLeft();
    if(c == nullptr){
        return p;
    }

    AVLNode<Key, Value>* temp = c->getRight(); 

    c->setRight(p);                                     
    p->setLeft(temp);

    AVLNode<Key, Value>* parent = p->getParent();           
    c->setParent(parent);     
    p->setParent(c);

    if(temp != nullptr){                               
        temp->setParent(p);
    }
    if(parent == nullptr){                         
        this->root_ = c;
    }else if(parent->getLeft() == p){          
        parent->setLeft(c);                    
    }else{                                          
        parent->setRight(c);
    }

    
    c->setBalance(static_cast<int8_t>(balance(c)));              
    p->setBalance(static_cast<int8_t>(balance(p)));                   
    return c;


}



template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertHelp(AVLNode<Key, Value>* r, const std::pair<const Key, Value> &new_item){

    if(r == nullptr){                                                                                       //tree is empty/base, allocate new root and return
        AVLNode<Key, Value>* inserted = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return inserted;
    }

    if(new_item.first == r->getKey()){                                                  //we found a duplicate
        r->setValue(new_item.second);                                                   //update stored value
        return r;
    }

    bool going_left = (new_item.first < r->getKey());                       //decide to go down left or right

    if(going_left){
        if(r->getLeft() != nullptr){                        //going down left, check if there is
            AVLNode<Key, Value>* left = insertHelp(r->getLeft(), new_item);             //keep going down
            r->setLeft(left);                                                           //update left child of r since possible rotations happen
            left->setParent(r);
        }else{
            AVLNode<Key, Value>* child = new AVLNode<Key, Value>(new_item.first, new_item.second, r);  //no left child so create here with new key, value, and parent being r
            r->setLeft(child);                                                                         //update
        }
    }else{          //going right
        if(r->getRight() != nullptr){
            AVLNode<Key, Value>* right = insertHelp(r->getRight(), new_item);
            r->setRight(right);
            right->setParent(r);
        }else{
            AVLNode<Key, Value>* child = new AVLNode<Key, Value>(new_item.first, new_item.second, r);
            r->setRight(child);
        }
    }

    r->setBalance(static_cast<int8_t>(balance(r))); 

    int tempBal = balance(r);

    if(tempBal < -1 && balance(r->getLeft()) <= 0){         //left left case, one rotate right
        r = rotateRight(r);
        return r;
    }
    if(tempBal > 1 && balance(r->getRight()) >= 0){         //right right case, one rotate left
        r = rotateLeft(r);
        return r;
    }
    if(tempBal > 1 && balance(r->getRight()) < 0){          //right left case, rotate right then rotate left
        r->setRight(rotateRight(r->getRight()));            //rotate right on right child first
        r->getRight()->setParent(r);                        //update parent
        r = rotateLeft(r);                                  //rotate left to balance
        return r;                                
    }
    if(tempBal < -1 && balance(r->getLeft()) > 0){          //left right case, rotate left then rotate right
        r->setLeft(rotateLeft(r->getLeft()));
        r->getLeft()->setParent(r);
        r = rotateRight(r);
        return r;
    }

    return r;           //returns r no rotations




}



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* r = static_cast<AVLNode<Key, Value>*>(this->root_);           //cast root_ to AVLNode to be used in insertHelp
    r = insertHelp(r, new_item);                                                        //call helper
    this->root_ = r;                                                                    //update new root if it changed from rotation

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* removed = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));          //cast Node to AVL, use internal find to find the node we want to remove
    //similar to bst remove
    if(removed == nullptr){
        return;                                         //don't do anything is key is not in tree
    }

    if(removed->getLeft() != nullptr && removed->getRight() != nullptr){        //checks for two children
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(removed));
        if(pred != nullptr){
            this->nodeSwap(removed, pred);                                                //swap
        }
    }

    AVLNode<Key, Value>* child = nullptr;      //save child
    
    if(removed->getLeft() != nullptr){
        child = removed->getLeft();             //left child
    }else if(removed->getRight() != nullptr){
        child = removed->getRight();                //right child
    }

    AVLNode<Key, Value>* parent = removed->getParent();        

    if(parent == nullptr){          //removed is the root, so now the child will be
        this->root_ = child;
    }else{
        if(parent->getLeft() == removed){
            parent->setLeft(child);             //if removed is left child, set left child of parent to child of removed to get rid of removed
        }else{
            parent->setRight(child);            //right child case
        }
    }

    if(child != nullptr){
        child->setParent(parent);       //update the child's parent if it is not nullptr
    }

    delete removed;     //free up


    //rebalancing with rotate    
    AVLNode<Key, Value>* subRoot = parent;              //start rebalancing from here

    while(subRoot != nullptr){                          //use while to walk up towards root

        subRoot->setBalance(static_cast<int8_t>(this->balance(subRoot)));

        int tempBal = balance(subRoot);

        if(tempBal < -1 && balance(subRoot->getLeft()) <= 0){         //left left case
            subRoot = rotateRight(subRoot);    
                    
        }else if(tempBal > 1 && balance(subRoot->getRight()) >= 0){         //right right case
            subRoot = rotateLeft(subRoot);
            
        }else if(tempBal > 1 && balance(subRoot->getRight()) < 0){          //right left case
            subRoot->setRight(rotateRight(subRoot->getRight()));            
            subRoot->getRight()->setParent(subRoot);                        
            subRoot = rotateLeft(subRoot);     
                                                                      
        }else if(tempBal < -1 && balance(subRoot->getLeft()) > 0){          //left right case
            subRoot->setLeft(rotateLeft(subRoot->getLeft()));
            subRoot->getLeft()->setParent(subRoot);
            subRoot = rotateRight(subRoot);
            
        }

        subRoot = subRoot->getParent();             //move up towards the root

    }


}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
