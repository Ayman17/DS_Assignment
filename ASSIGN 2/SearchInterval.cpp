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

    void InsertInterval(int high, int low)
    {
        if (root == nullptr)
        {
            root = new Node(high, low, nullptr);
            return;
        }

        Node *current = root;
        Node *parent = nullptr;

        while (current != nullptr)
        {
            if ((low < parent->low) || (low == parent->low && high < parent->high))
            {
                parent = current;
                current = current->left;
            }
            else
            {
                parent = current;
                current = current->right;
            }
        }

        if ((low < parent->low) || (low == parent->low && high < parent->high))
        {
            parent->left = new Node(high, low, parent);
        }
        else
        {
            parent->right = new Node(high, low, parent);
        }

        updateMax(parent);
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