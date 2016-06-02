#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

using namespace std;

class TreeNode {
    public:
        TreeNode* left;
        TreeNode* right;
        int height;
        int key;

        TreeNode(int value) {
            this->key = value;
            this->left = NULL;
            this->right = NULL;
        }

        int balanceFactor() {
            int hRight = (this->right ? this->right->getHeight() : 0);
            int hLeft = (this->left ? this->left->getHeight() : 0);


            //cout << "Balance heights (right - left): " << hRight << " - " << hLeft << " = " << hRight - hLeft << endl;
            return hRight - hLeft;
        }

        int getHeight() {
            int hRight = (this->right ? this->right->getHeight() : 0);
            int hLeft = (this->left ? this->left->getHeight() : 0);

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

        TreeNode* insert(int value) {
            if (value < this->key) {
                //cout << "Moving to left kid" << endl;
                if (!this->left) {
                    this->left = new TreeNode(value);
                }
                else {
                    this->left = this->left->insert(value);
                }
            }
            else {
                //cout << "Moving to right kid" << endl;
                if (!this->right) {
                    this->right = new TreeNode(value);
                }
                else {
                    this->right = this->right->insert(value);
                }
            }

            return this->balance();
        }

        TreeNode* getMin() {
            if (this->left) {
                return this->left->getMin();
            }
            //cout << "Got leftMost node. Its key is: " << node->key << endl;

            return this;
        }

        static TreeNode* removeLeftMost(TreeNode* node) {
            if (node->left == 0) { //Check out this if statement
                return node->right;
            }


            node->left = TreeNode::removeLeftMost(node->left);

            return node->balance();
        }

        static TreeNode* remove(TreeNode* node, int value) {
            if(!node) {
                return 0;
            }

            if(value < node->key) {
                node->left = TreeNode::remove(node->left, value);
            }
            else if (value > node->key) {
                node->right = TreeNode::remove(node->right, value);
            }
            else {
                TreeNode* nodeA = node->left; //q = nodeA
                TreeNode* nodeB = node->right;

                //cout << "Removing node with value " << node->key << endl;
                delete node;

                if (!nodeB) {
                    return nodeA;
                }
                TreeNode* min = nodeB->getMin();
                min->right = TreeNode::removeLeftMost(nodeB);
                min->left = nodeA;

                return min->balance();
            }

            return node->balance();
        }

        int countNodes() {
            int count = 1;

            count += (this->left ? this->left->countNodes() : 0);
            count += (this->right ? this->right->countNodes() : 0);

            return count;
        }

};

int main() {
    ifstream in("input.txt");
    int id, link;
    map<unsigned int, TreeNode *> trees;

    while (!in.eof()) {
        in >> id >> link;

        if (trees.find(id) != trees.end()) {
            //cout << "To already created tree for id: " << id << " adding link: " << link << endl;
            trees[id] = trees[id]->insert(link);
        }
        else {
            //cout << "Creating new key with id: " << id << " and adding link: " << link << endl;
            trees[id] = new TreeNode(link);
        }
    }

    in.close();

    cout << trees[0]->countNodes() << endl;

    return 0;
}
