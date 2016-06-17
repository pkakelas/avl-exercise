#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <sstream>

using namespace std;

class TreeNode {
    public:
        TreeNode* left;
        TreeNode* right;
        TreeNode* value;
        int height;
        int key;

        TreeNode(int key, TreeNode* value = NULL) {
            this->key = key;
            this->value = value;
            this->left = NULL;
            this->right = NULL;
        }

        TreeNode* insertKey(int key, TreeNode* value = NULL) {
            if (key < this->key) {
                if (!this->left) {
                    //cout << "Creating left node with parent (" << this->key << ")" << endl;
                this->left = new TreeNode(key, value);
                }
                else {
                    //cout << "Moving to left kid" << endl;
                    this->left = this->left->insertKey(key, value);
                }
            }
            else {
                if (!this->right) {
                    //cout << "Creating right node with parent (" << this->key << ")" << endl;
                    this->right = new TreeNode(key, value);
                }
                else {
                    //cout << "Moving to right kid" << endl;
                    this->right = this->right->insertKey(key, value);
                }
            }

            return this->balance();
        }

        TreeNode* getNode(int key) {
            //cout << "Key = " << this->key << endl;
            if (this->key == key) {
                //cout << "found" << endl;
                return this;
            }
            else if (key > this->key && this->right) {
                //cout << "going bigger" << endl;
                return this->right->getNode(key);
            }
            else if (key < this->key && this->left) {
                //cout << "going smaller" << endl;
                return this->left->getNode(key);
            }
            return NULL;
        }

        TreeNode* getMin() {
            if (this->left) {
                return this->left->getMin();
            }
            //cout << "Got leftMost node. Its key is: " << node->key << endl;

            return this;
        }

        TreeNode* removeMin() {
            if (!this->left && !this->right) {
                return NULL;
            }
            else if (this->left == NULL) {
                return this->right;
            }

            this->left = this->left->removeMin();

            return this->balance();
        }

        string exportTreeData(int id) {
            TreeNode* tree = this;

            ostringstream key;

            key << id << " " << this->countNodes();
            while (tree != NULL) {
                key << " " << tree->getMin()->key;
                tree = tree->removeMin();
            }

            return key.str();
        }

        static TreeNode* remove(TreeNode* node, int key) {
            if(key < node->key) {
                node->left = (node->left ? TreeNode::remove(node->left, key) : NULL);
            }
            else if (key > node->key) {
                node->right = (node->right ? TreeNode::remove(node->right, key) : NULL);
            }
            else {
                TreeNode* nodeA = node->left; //q = nodeA
                TreeNode* nodeB = node->right;

                //cout << "Removing node with key " << node->key << endl;
                delete node;

                if (!nodeB) {
                    return nodeA;
                }
                TreeNode* min = nodeB->getMin();
                min->right = nodeB->removeMin();
                min->left = nodeA;

                return min->balance();
            }

            return node->balance();
        }

        int countNodes() {
            int count = 1;

            count += this->left ? this->left->countNodes() : 0;
            count += this->right ? this->right->countNodes() : 0;

            return count;
        }

    private:
        int balanceFactor() {
            int hRight = this->right ? this->right->getHeight() : 0;
            int hLeft = this->left ? this->left->getHeight() : 0;

            //cout << "Balance heights (right - left): " << hRight << " - " << hLeft << " = " << hRight - hLeft << endl;
            return hRight - hLeft;
        }

        int getHeight() {
            int hRight = this->right ? this->right->getHeight() : 0;
            int hLeft = this->left ? this->left->getHeight() : 0;

            int max = hLeft > hRight ? hLeft : hRight;

            return max + 1;
        }

        TreeNode* rotate(string orientation) { //operation: right || left
            TreeNode* nodeB = NULL;

            //cout << "Rotating " << orientation << " the node (" << this->key << ")" << endl;
            if (orientation == "right") {
                nodeB = this->left;
                this->left = nodeB->right;
                nodeB->right = this;
            }
            else {
                nodeB = this->right;
                this->right = nodeB->left;
                nodeB->left = this;
            }

            return nodeB;
        }

        TreeNode* balance() {
            //cout << endl << "Initiating balance" << endl << "====================" << endl;
            int balance = this->balanceFactor();

            if (balance == 2) {
                //cout << "Leaning right" << endl;
                if (this->right->balanceFactor() < 0) {
                    this->right = this->right->rotate("right");
                }

                return this->rotate("left");
            }

            if (balance == -2) {
                //cout << "Leaning left" << endl;
                if (this->left->balanceFactor() > 0) {
                    this->left = this->left->rotate("left");
                }

                return this->rotate("right");
            }

            //cout << "Balanced this with key: " << this->key << endl;
            return this;
        }
};

TreeNode*
importTreesFromFile(string file) {
    ifstream in(file.c_str());
    unsigned int id, link;

    //Initializing basic tree
    in >> id >> link;
    TreeNode* basicTree = new TreeNode(id, new TreeNode(link));
    //cout << "Created new tree for id: " << basicTree->key << " with link: " << basicTree->value->key << endl;

    while (in >> id >> link) {
        TreeNode* node = basicTree->getNode(id);

        if (node) {
            //cout << "To already created tree for id: " << node->key << " adding link: " << link << endl;
            node->value = node->value->insertKey(link);
        }
        else {
            //cout << "Created new tree for id: " << id << " with link: " << link << endl;
            basicTree = basicTree->insertKey(id, new TreeNode(link));
        }
    }

    in.close();

    return basicTree;
}

int main() {
    ifstream in("commands.txt");
    string command, file;
    int id, link;
    TreeNode* basicTree = NULL;

    while (in >> command) {
        if (command == "READ_DATA") {
            in >> file;
            cout << "Reading data from file: " << file << endl;

            basicTree = importTreesFromFile("input.txt");
        }
        if (command == "INSERT_LINK") {
            in >> id >> link;
            cout << "Inserting link from " << id << " to " << link << endl;

            TreeNode* node = basicTree->getNode(id);
            node->value = node->value->insertKey(link);
        }
        if (command == "DELETE_LINK") {
            in >> id >> link;
            cout << "Deleting link from " << id << " to " << link << endl;

            if (!basicTree->getNode(id)) {
                cout << "AVL tree with id: " << id << "not found" << endl;
            }

            TreeNode* node = basicTree->getNode(id);
            node->value = TreeNode::remove(node->value, link);
        }
        if (command == "WRITE_INDEX") {
            in >> file;
            cout << "Writing data to file: " << file << endl;

            ofstream out(file.c_str());

            int count = basicTree->countNodes();

            for (int i = 0; i < count; ++i) {
                TreeNode* minNode = basicTree->getMin();
                out << minNode->value->exportTreeData(minNode->key) << endl;
                basicTree = TreeNode::remove(basicTree, basicTree->getMin()->key);
            }

            out.close();
        }
    }

    return 0;
}
