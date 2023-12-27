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

    Node(int high, int low, Node *parent)
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
            root = new Node(high, low, nullptr);
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
            current->left = new Node(high, low, current);
            current->left->parent = current;
            updateMax(current->left);
        }
        else
        {
            current->right = new Node(high, low, current);
            current->right->parent = current;
            updateMax(current->right);
        }
        cout << "done";
    }

    void updateMax(Node *current)
    {
        int newMax = current->high;
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

    void SearchInterval(int high, int low)
    {
        if (root == nullptr)
        {
            return;
        }

        Node *miniumLow = root;
        Node *current = root;
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
    cout << "hahahaha";
}