#include <iostream>

using namespace std;

class Node
{
public:
    int high;
    int low;
    int max = -1;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    Node(int low, int high, Node *parent)
    {
        this->high = high;
        this->low = low;
        this->parent = parent;
        this->max = high;
    }
};

class IntervalTree
{
public:
    Node *root = nullptr;

    void InsertInterval(int low, int high)
    {
        if (root == nullptr)
        {
            root = new Node(low, high, nullptr);
            return;
        }

        Node *current = root;
        Node *nodeParent = nullptr;

        while (current != nullptr)
        {
            if ((low < current->low) || (low == current->low && high < current->high))
            {
                nodeParent = current;
                current = current->left;
            }
            else
            {
                nodeParent = current;
                current = current->right;
            }
        }

        current = nodeParent;

        if ((low < current->low) || (low == current->low && high < current->high))
        {
            current->left = new Node(low, high, current);
            current->left->parent = current;
        }
        else
        {
            current->right = new Node(low, high, current);
            current->right->parent = current;
        }
        updateMax(nodeParent, high);
    }

    void updateMax(Node *current, int newMax)
    {
        while (current != nullptr)
        {
            if (current->max >= newMax)
            {
                break;
            }

            current->max = newMax;
            current = current->parent;
        }
    }

    Node* SearchInterval(int low, int high)
    {
        if (root == nullptr)
        {
            throw "Tree is empty";
        }

        Node* target = new Node(low, high, nullptr);

        return searchInterval(root, target);

    }

    Node* searchInterval(Node *node, Node* target) {
        if (node == nullptr || !canOverLap(node, target)) {
            return nullptr;
        }

        Node* minimum = nullptr;

        if (isOverlapping(target, node)) {
            minimum = node;
        }

        Node* currentLeft = searchInterval(node->left, target);
        Node* currentRight = searchInterval(node->right, target);

        if (currentLeft != nullptr && isOverlapping(currentLeft, target)) {
            if (minimum == nullptr || currentLeft->low < minimum->low) {
                minimum = currentLeft;
            }
        } 

        if (currentRight != nullptr && isOverlapping(currentRight, target)) {
            if (minimum == nullptr || currentRight->low < minimum->low) {
                minimum = currentRight;
            }
        } 

        return minimum;
    }

     

    bool canOverLap(Node *node, Node* target) {
        if (node->low > target->low && node->left == nullptr) {
            return false; //15, 20, max 40
        }
        if (node->max < target->high) {
            return false;
        }

        return true;
    }

    bool isOverlapping(Node *first, Node *second)
    {
        return (first->low <= second->high && second->low <= first->high);
    }
};

int main()
{
    IntervalTree tree;
    tree.InsertInterval(15, 20);
    tree.InsertInterval(10, 30);
    tree.InsertInterval(17, 19);
    tree.InsertInterval(5, 20);
    tree.InsertInterval(12, 15);
    tree.InsertInterval(30, 40);

    Node* result = tree.SearchInterval(5, 40);
    if (result == nullptr) {
        cout << "null";
    } else {
        cout << result->low;

    }
}