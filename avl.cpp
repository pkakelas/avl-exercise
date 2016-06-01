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

        static int balanceFactor(TreeNode* node) {
            return getHeight(node->right) - getHeight(node->left);
        }

        static int getHeight(TreeNode* node) {
            if (!node) {
                return 0;
            }

            int leftHeight = getHeight(node->left);
            int rightHeight = getHeight(node->right);

            int max = leftHeight > rightHeight ? leftHeight : rightHeight;

            return max + 1;
        }

        static TreeNode* rotate(TreeNode* nodeA, string orientation) { //operation: right || left
            TreeNode* nodeB = NULL;

            //cout << "Rotating " << orientation << " the node (" << nodeA->key << ")" << endl;

            if (orientation == "right") {
                nodeB = nodeA->left;
                nodeA->left = nodeB->right;
                nodeB->right = nodeA;
            }
            else {
                nodeB = nodeA->right;
                nodeA->right = nodeB->left;
                nodeB->left = nodeA;
            }

            return nodeB;
        }

		static TreeNode* balance(TreeNode* node) {
            //cout << endl << "Initiating balance" << endl << "====================" << endl;

            int balance = TreeNode::balanceFactor(node);

            if (balance == 2) {
                //cout << "Leaning right" << endl;
                if(TreeNode::balanceFactor(node->right) < 0) {
                    node->right = TreeNode::rotate(node->right, "right");
                }

                return TreeNode::rotate(node, "left");
            }

            if (balance == -2) {
                //cout << "Leaning left" << endl;
                if(balanceFactor(node->left) > 0) {
                    node->left = TreeNode::rotate(node->left, "left");
                }

                return TreeNode::rotate(node, "right");
            }

            //cout << "Balanced node with key: " << node->key << endl;

            return node;
        }

		static TreeNode* insert(TreeNode* node, int value) {
            if (!node) {
                //cout << "Node (" << value << ") inserted"<< endl;
                return new TreeNode(value);
            }
			if (value < node->key) {
                //cout << "Moving to left kid" << endl;
                node->left = TreeNode::insert(node->left, value);
			}
			else {
                //cout << "Moving to right kid" << endl;
                node->right = TreeNode::insert(node->right, value);
			}

            return TreeNode::balance(node);
		}

		static TreeNode* getLeftMost(TreeNode* node) {
            if (node->left) {
                return TreeNode::getLeftMost(node->left);
            }

            //cout << "Got leftMost node. Its key is: " << node->key << endl;

            return node;
		}

		static TreeNode* removeLeftMost(TreeNode* node) {
			if (node->left == 0) { //Check out this if statement
				return node->right;
            }


			node->left = TreeNode::removeLeftMost(node->left);

			return TreeNode::balance(node);
		}

		int countNodes() {
            int count = 1;

            count += (this->left ? this->left->countNodes() : 0);
            count += (this->right ? this->right->countNodes() : 0);

            return count;
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
                TreeNode* min = TreeNode::getLeftMost(nodeB);
                min->right = TreeNode::removeLeftMost(nodeB);
                min->left = nodeA;

                return TreeNode::balance(min);
            }

            return TreeNode::balance(node);
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
            trees[id] = TreeNode::insert(trees[id], link);
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
