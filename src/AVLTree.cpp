#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <queue>

using namespace std;

struct Node {
    string name;
    int id;
    Node *left = nullptr;
    Node *right = nullptr;
    int height = 0;

    /**
     * Default constructor
     */
    Node() {

    }

    /**
     * Constructors that accepts name and id
     * @param name, a valid and parsed name
     * @param id , a valid ID that contains only numbers, 8 digits
     */
    Node(string name, int id) {
        this->name = name;
        this->id = id;
    }

    /**
     * gets the height of the AVL tree
     * @param accepts node (usually itself)
     * @return int, the height of the node of current node
     */
    int calHeight(Node *node) {

        //If reaches the end, returns 0
        if (node == nullptr) {
            return -1;
        } else {

            //Get the height
            int rHeight = calHeight(node->right) + 1;
            int lHeight = calHeight(node->left) + 1;

            //Compare the height and return greater one
            if (rHeight > lHeight) {
                return(rHeight);
            } else {
                return(lHeight);
            }
        }
    }
};

class AVLTree {
private:
    Node *head = nullptr;

    /**
     * Left rotation helper function that aids in balancing
     * @param parent
     * @return root node that's pivoted
     */
    Node* leftRotation(Node* parent) {
        Node* root = parent->right;
        parent->right = root->left;
        root->left = parent;

        //Updating the height
        root->calHeight(root);
        root->calHeight(root->left);
        root->calHeight(root->right);
        return root;
    }

    /**
     * Right rotation helper function that aids in balancing
     * @param parent node
     * @return root node that's pivoted
     */
    Node* rightRotation(Node* parent) {
        Node* root = parent->left;
        parent->left = root->right;
        root->right = parent;

        //Updating the height
        root->calHeight(root);
        root->calHeight(root->left);
        root->calHeight(root->right);
        return root;
    }

    /**
     * The main balancing helper function that calls other other functions
     * like left/right rotate. Called by insert and remove
     * @param factor The balance factor
     * @param parent The parent node that triggered this
     */
    Node* balance(int factor, Node* parent) {

        //Right heavy
        if (factor == -2) {
            int innerFactor = parent->calHeight(parent->right->left) - parent->calHeight(parent->right->right);

            //Right right or right left
            if (innerFactor == -1) {

                //RR, moves top node to child's left
                return leftRotation(parent);
            } else {

                //RL move
                parent->right = rightRotation(parent->right);
                return leftRotation(parent);
            }

            //Left heavy
        } else if (factor == 2) {
            int innerFactor = parent->calHeight(parent->left->left) - parent->calHeight(parent->left->right);

            //Left right or left left
            if (innerFactor == 1) {

                //LR
                return rightRotation(parent);
            } else {

                //LL
                parent->left = leftRotation(parent->left);
                Node* node = rightRotation(parent);
                return node;
            }
        }
        return nullptr;
    }

    /**
     * A helper function for remove that gets the inorder successor of
     * a node, given its right subtree as a node
     * @param root, the right subtree
     * @param ID
     * @param name
     * @return the root entered
     */
    Node* inorderSuccessor(Node* root, int &ID, string &name, Node* &successor) {

        //If the left node of the right sub is nullptr, we hit it!
        if (root->left == nullptr) {

            //Get the values
            ID = root->id;
            name = root->name;
            successor = root;
            return nullptr;

            //Otherwise, keep going left!
        } else {
            root->left = inorderSuccessor(root->left, ID, name, successor);
        }
        return root;
    }

    /**
     * Helper function for cleaning up the nodes, used by destructor
     * @param root, head of the tree
     */
    void deleteNode(Node* root) {
        if (root == nullptr) {
            return;
        } else {
            deleteNode(root->left);
            deleteNode(root->right);
            delete root;
        }
    }

public:
    /**
     * Gets the private head of the tree
     * @return The head of the tree
     */
    Node* getHead() {
        return this->head;
    }

    /**
     * Sets the head of the tree
     * @param new node, sets the new node
     */
    void setHead(Node* newHead) {
        this->head = newHead;
    }

    /**
     * Insert a node into the AVL tree
     * @param root, root node
     * @param name, string name
     * @param ID, int id
     * @return The node with the name and id that was inserted
     */
    Node* insert(Node* root, string name, int ID) {

        //Balancing
        if (root==nullptr) {
            std::cout << "successful" << std::endl;
            return new Node(name, ID);

            //Duplicate values found, print unsuccessful, return nullptr
        } else if (root->id== ID) {
            std::cout << "unsuccessful" << std::endl;
            return nullptr;
        }

        //Navigating the tree based on ID
        if (ID < root->id){
            root->left = insert(root->left, name, ID);
        } else {
            root->right = insert(root->right, name, ID);
        }

        //Updating height, check for balances
        root->calHeight(root);
        if ((root->calHeight(root->left) - root->calHeight(root->right)) == 2){
            root = balance(2, root);
        } else if ((root->calHeight(root->left) - root->calHeight(root->right)) == -2){
            root = balance(-2, root);
        }
        return root;
    }

    /**
     * Deletes an node by the ID
     * @param root
     * @param ID
     * @param deleted
     * @return Root of the tree
     */
    Node* remove(Node* root, int ID, bool &deleted) {

        //Do nothing if root is null
        if (root==nullptr) {
            return nullptr;
        }

        //Navigating the tree
        if (ID < root->id){
            root->left = remove(root->left,ID, deleted);
        } else if (ID > root->id){
            root->right = remove(root->right, ID, deleted);

            //Return inorder successor
        } else if (ID == root->id) {
            deleted = true;

            //if both is null, just delete
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;

                //if one of them is not null
            } else if (root->left == nullptr || root->right == nullptr) {
                Node* next;
                if (root->left != nullptr) {
                    next = root->left;
                } else {
                    next = root->right;
                }
                delete root;
                return next;

                //But if both are valid nodes
            } else {
                int newID = 0;
                string newName="";

                //Pass in the root->right, keep going left!
                Node* successor;
                inorderSuccessor(root->right, newID, newName, successor);

                //The left of the root and the right of the successor
                root->id = newID;
                root->name = newName;
                root->right = remove(root->right, newID, deleted);
                return root;
            }
        }

        //Updating height, check for balances
        root->calHeight(root);
        if ((root->calHeight(root->left) - root->calHeight(root->right)) == 2){
            root = balance(2, root);
        } else if ((root->calHeight(root->left) - root->calHeight(root->right)) == -2){
            root = balance(-2, root);
        }
        return root;
    }

    /**
     * Deletes at nth node in traversal order
     * @param root
     * @param nth
     * @param counter
     * @param deleted
     */
    void remove(Node* root, int nth, int &counter, bool &deleted) {

        //If the root is null, do nothing
        if (root == nullptr) {
            std::cout << "" ;

            //Otherwise, start traversing until you hit the right counter
        } else {
            remove(root->left, nth, counter, deleted);

            //If the counter is equal to the specified nth
            if (nth == counter) {

                //One for the helper function, other for own function
                bool deleted2 = true;
                deleted = true;
                remove(getHead(), root->id, deleted2);
                root = nullptr;
            }
            counter++;
            if (root != nullptr) {
                remove(root->right, nth, counter, deleted);
            }
        }
    }


    /**
     * Prints the inorder traversal of the tree
     * @param root,  head of the tree
     * @param nodes, main function node queue
     */
    void inorder(Node* root, queue<Node*> &nodes) {
        if (root == nullptr) {
            std::cout << "";
        } else {
            inorder(root->left, nodes);
            nodes.push(root);
            inorder(root->right, nodes);
        }
    }

    /**
     * Prints the preorder traversal of the tree
     * @param root,  head of the tree
     * @param nodes, main function node queue
     */
    void preorder(Node* root, queue<Node*> &nodes) {
        if (root == nullptr) {
            std::cout << "";
        } else {
            nodes.push(root);
            preorder(root->left, nodes);
            preorder(root->right, nodes);
        }
    }

    /**
     * Prints the postorder traversal of the tree
     * @param root,  head of the tree
     * @param nodes, main function node queue
     */
    void postorder(Node* root, queue<Node*> &nodes) {
        if (root == nullptr) {
            std::cout << "";
        } else {
            postorder(root->left, nodes);
            postorder(root->right, nodes);
            nodes.push(root);
        }
    }

    /**
     * Search through tree given ID
     * @param root, head of the tree
     * @param ID, valid parsed 8 digit ID
     * @param found, passed by reference
     * @return Node which matches the corresponding ID
     */
    void search(Node* root, int ID, bool &found) {
        if (root == nullptr) {
            return;
        }

        //Navigating the tree
        if (ID < root->id) {
            search(root->left, ID, found);
        } else if (ID > root->id) {
            search(root->right, ID, found);

            //If equals, return that node
        } else if (ID == root->id){
            found = true;
            std::cout << root->name << std::endl;
        }
    }

    /**
     * Finds and print the associated ID given a name
     * @param root, head of the tree
     * @param name, valid parsed student name
     * @param found, boolean passed by reference
     */
    void search(Node* root, string name, bool &found) {
        if (root == nullptr) {
            std::cout << "";

            //Return name if found, otherwise found = false by default
        } else {
            if (root->name == name) {
                found = true;
                cout << root->id << std::endl;
            }

            //Traverse through all branches
            search(root->left, name, found);
            search(root->right, name, found);
        }

    }


    /**
     * Destructor, deletes all nodes
     */
    ~AVLTree() {
        deleteNode(getHead());
    }


    /**
     * For debugging purposes only, prints the tree
     * @param root, of the tree
     */
    void printTree(Node* root) {
        if (root == nullptr) {
            return;
        } else {
            std::cout << "Root: " << root->id << std::endl;
            if (root->left!=nullptr)
                std::cout << "left: " << root->left->id << std::endl;
        }
        if (root->right!=nullptr) {
            std::cout << "right: " << root->right->id << std::endl;
        }
        printTree(root->left);
        printTree(root->right);
    }
};