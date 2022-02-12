#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <queue>
#include "AVLTree.cpp"

using namespace std;

/**
 * Validates the ID of the student number
 * @param ID, 8 digit integer
 * @return boolean, true or false
 */
bool validID(int ID) {
    //Check if ID is 8 digits
    if (ID >= 10000000 && ID <= 99999999 ){
        return true;
    }
    return false;
}

/**
 * Validates the name of the student
 * @param name, a string input
 * @return boolean, true or false
 */
bool validName(string name) {
    //Makes sure that there's only letters and spaces
    for (int x = 0; x < name.length(); x++) {
        if (!isalpha(name.at(x))) {
            if (name.at(x) != ' ') {
                return false;
            }
        }
    }
    return true;
}

int main() {

    //Initializing variables
    AVLTree tree = AVLTree();
    int numInstr, ID, nth;
    string line, command, name;
    queue<Node*> nodes;
    cin >> numInstr;

    //For the clean up (extra space)
    getline(cin, line);
    for (int x = 0; x < numInstr; x++) {

        //Gets the line from input
        getline(cin, line);

        //Creates input stream
        istringstream in(line);

        //Read command
        in >> command;

        //If command is insert
        if (command == "insert") {

            //Gets the name, remove the quotes, first one to swallow the space
            getline(in, name, '"');
            getline(in, name, '"');
            in >> ID;
            if (validName(name) && validID(ID)){

                //First check if the root node exists, if not, initialize the root node
                if (tree.getHead() == nullptr) {
                    std::cout << "successful" << std::endl;
                    Node* head = new Node(name, ID);
                    tree.setHead(head);

                //Otherwise insert as intended
                } else {

                    //Set the head in case the head gets changed
                    tree.setHead(tree.insert(tree.getHead(), name, ID));
                }
            } else {

                //Fails
                std::cout << "unsuccessful" << std::endl;
            }

        //Remove command
        } else if (command == "remove"){
            in >> ID;
            if (validID(ID)) {

                //Search for the ID
                bool deleted = false;

                //Set the head if the root changes
                tree.setHead(tree.remove(tree.getHead(), ID, deleted));
                if (deleted == false) {
                    std::cout << "unsuccessful" << std::endl;
                } else {
                    std::cout << "successful" << std::endl;
                }
            } else {

                //Fails otherwise
                std::cout << "unsuccessful" << std::endl;
            }

        //Search command
        } else if (command == "search") {

            /*
             * First getline swallows up the space before first quotes
             * then get rest of the letters up until last quote
             * to include the spaces
             */
            getline(in, name, '"');
            getline(in, name, '"');
            if (validName(name)) {

                //Search for the name
                bool found = false;
                tree.search(tree.getHead(), name, found);
                if (found != true) {
                    std::cout << "unsuccessful" << std::endl;
                }
            } else {

                //Possibly an ID instead, have try catch in case for getting string->int
                try {
                    if (validID(stoi(name))) {
                        ID = stoi(name);

                        //Search for the ID, pass in boolean by reference
                        bool found = false;
                        tree.search(tree.getHead(), ID, found);
                        if (found != true) {
                            std::cout << "unsuccessful" << std::endl;
                        }
                    } else {

                        //Fails otherwise
                        std::cout << "unsuccessful" << std::endl;
                    }
                } catch (...) {

                    //If input still invalid
                    std::cout << "unsuccessful" << std::endl;
                }
            }

        //Print inorder command
        } else if (command == "printInorder") {

            //Prints the inorder traversal of tree
            tree.inorder(tree.getHead(), nodes);
            int size = nodes.size();
            for (int x = 0; x < size; x++) {
                if (x != size - 1) {
                    std::cout << nodes.front()->name << ", ";
                    nodes.pop();
                } else {
                    std::cout << nodes.front()->name << std::endl;
                    nodes.pop();
                }
            }

        //Print preorder command
        } else if (command == "printPreorder") {

            //Prints the preorder traversal of tree
            tree.preorder(tree.getHead(), nodes);
            int size = nodes.size();
            for (int x = 0; x < size; x++) {
                if (x != size - 1) {
                    std::cout << nodes.front()->name << ", ";
                    nodes.pop();
                } else {
                    std::cout << nodes.front()->name << std::endl;
                    nodes.pop();
                }

            }

        //Print postorder command
        } else if (command == "printPostorder") {

            //Prints the postorder traversal of tree
            tree.postorder(tree.getHead(), nodes);
            int size = nodes.size();
            for (int x = 0; x < size; x++) {
                if (x != size - 1) {
                    std::cout << nodes.front()->name << ", ";
                    nodes.pop();
                } else {
                    std::cout << nodes.front()->name << std::endl;
                    nodes.pop();
                }
            }

        //Print level count command
        } else if (command == "printLevelCount") {

            //Prints the level count of tree
            if (tree.getHead()!=nullptr) {
                std::cout << tree.getHead()->calHeight(tree.getHead())+1 << std::endl;
            } else {
                std::cout << 0 << std::endl;
            }

        //Remove inorder command
        } else if (command == "removeInorder") {

            //Removes nth node in inorder sequence
            in >> nth;
            bool deleted = false;
            int counter = 0;

            //Function call
            tree.remove(tree.getHead(), nth, counter, deleted);
            if (deleted == false) {
                std::cout << "unsuccessful" << std::endl;
            } else {
                std::cout << "successful" << std::endl;
            }
        }
    }
    return 0;
}
