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

        if (isOverlapping(node, target)) {
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
        if (node->low > target->high && node->left == nullptr) {
            return false; //15, 20, max 40
        }
        if (node->max < target->low) {
            return false;
        }

        return true;
    }

    bool isOverlapping(Node *first, Node *second)
    {
        return (first->low <= second->high && second->low <= first->high);
    }
};

#include <iostream>

// Your Node and IntervalTree class definitions go here.
// ...

class IntervalTreeTester {
private:
    static const int NUM_TESTS = 3;
    static const int NUM_INTERVALS = 10;

    void checkTest(int low, int high, const char *expected, IntervalTree tree)
    {
        Node *result = tree.SearchInterval(low, high);
        cout << "Searching for [" << low << ", " << high << "], ";
        if (result == nullptr)
        {
            cout << "Result: Does not exsist, Expected: " << expected << endl;
        }
        else
        {
            cout << "Result: " << result->low << " Expected: " << expected << endl;
        }
    }

    void test1()
    {
        cout << "Test 1: " << endl;

        IntervalTree tree;
        tree.InsertInterval(10, 30);
        tree.InsertInterval(20, 40);
        tree.InsertInterval(0, 15);
        tree.InsertInterval(25, 35);
        tree.InsertInterval(5, 15);

        const char *doesntExsist = "Does not exsist";

        checkTest(22, 87, "10", tree);
        checkTest(48, 59, doesntExsist, tree);
        checkTest(20, 42, "10", tree);
        checkTest(33, 90, "20", tree);
        checkTest(38, 92, "20", tree);
        checkTest(50, 77, doesntExsist, tree);
        checkTest(13, 15, "0", tree);
        checkTest(1, 17, "0", tree);
        checkTest(24, 51, "10", tree);
        checkTest(85, 100, doesntExsist, tree);

        cout << endl;
    }

    void test2()
    {
        cout << "Test 2: " << endl;

        IntervalTree tree;
        // Insert some new intervals
        tree.InsertInterval(5, 25);
        tree.InsertInterval(15, 35);
        tree.InsertInterval(40, 60);
        tree.InsertInterval(70, 90);
        tree.InsertInterval(50, 70);

        // For this example, using a simple message string to indicate an expected result
        const char *doesntExist = "Does not exist";

        // Perform checks for various intervals
        checkTest(3, 10, "5", tree);          // Expected to overlap with interval 5-25
        checkTest(36, 45, "40", tree); // No overlap expected
        checkTest(55, 65, "40", tree);        // Expected to overlap with interval 40-60
        checkTest(75, 95, "70", tree);        // Expected to overlap with interval 70-90
        checkTest(30, 50, "15", tree);        // Expected to overlap with interval 15-35 and 50-70
        checkTest(42, 58, "40", tree);        // Expected to overlap with interval 40-60
        checkTest(22, 23, "5", tree);         // Expected to overlap with interval 5-25
        checkTest(65, 85, "50", tree);        // Expected to overlap with interval 70-90
        checkTest(27, 37, "15", tree);        // Expected to overlap with interval 15-35
        checkTest(10, 20, "5", tree);         // Expected to overlap with interval 5-25 and 15-35

        cout << endl;
    }

    void test3()
    {
        cout << "Test 3: " << endl;

        IntervalTree tree;
        // Insert some new intervals
        tree.InsertInterval(20, 50);
        tree.InsertInterval(10, 60);
        tree.InsertInterval(30, 40);
        tree.InsertInterval(55, 95);
        tree.InsertInterval(5, 15);

        // For this example, using a simple message string to indicate an expected result
        const char *doesntExist = "Does not exist";

        // Perform checks for various intervals
        checkTest(0, 10, "5", tree);          // Expected to overlap with interval 5-15
        checkTest(16, 19, "10", tree); // No overlap expected
        checkTest(35, 45, "10", tree);        // Expected to overlap with intervals 20-50 and 30-40
        checkTest(56, 100, "10", tree);       // Expected to overlap with interval 55-95
        checkTest(25, 30, "10", tree);        // Expected to overlap with interval 20-50
        checkTest(60, 70, "10", tree);        // Expected to overlap with interval 10-60
        checkTest(1, 4, doesntExist, tree);   // No overlap expected
        checkTest(96, 99, doesntExist, tree); // No overlap expected
        checkTest(45, 75, "10", tree);        // Expected to overlap with intervals 10-60 and 55-95
        checkTest(65, 90, "55", tree);        // Expected to overlap with interval 55-95

        cout << endl;
    }

public:
    void run() {
        test1();
        test2();
        test3();
    }
};

int main()
{
    IntervalTreeTester tester;
    tester.run(); // Run all the tests
    return 0;
}