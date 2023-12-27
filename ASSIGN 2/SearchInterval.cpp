#include <iostream>

using namespace std;

class Node
{
public:

    int high;       // high: upper bound of the interval
    int low;        // low: lower bound of the interval
    int max = -1;   // max: the maximum upper bound in the subtree rooted at this node, initialized to -1
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

    // Function to insert a new interval into the tree.
    void InsertInterval(int low, int high)
    {
        // If the tree is empty, create the root node with the provided interval.
        if (root == nullptr)
        {
            root = new Node(low, high, nullptr);
            return;
        }

        // Start from the root and find the correct insertion place.
        Node *current = root;
        Node *nodeParent = nullptr;

        // Traverse the tree and find the parent node under which to insert the new node.
        while (current != nullptr)
        {
            nodeParent = current;
            if (low < current->low || (low == current->low && high < current->high))
            {
                // Move to the left if the interval goes to the left sub-tree
                current = current->left;
            }
            else
            {
                // Move to the right otherwise
                current = current->right;
            }
        }

        // Assign the current position to the last node checked.
        current = nodeParent;

        // Check once more to see where the new node will be the left or right child.
        if (low < current->low || (low == current->low && high < current->high))
        {
            // If going to the left subtree, insert new node to the left.
            current->left = new Node(low, high, current);
        }
        else
        {
            // If going to the right subtree, insert new node to the right.
            current->right = new Node(low, high, current);
        }

        // After insertion, update the 'max' property up to the root as needed.
        updateMax(nodeParent, high);
    }

    // function to update the 'max' field after inserting a new interval.
    void updateMax(Node *current, int newMax)
    {
        // Traverses up the tree starting from the inserted node's parent.
        while (current != nullptr)
        {
            if (current->max >= newMax)
            {
                // If the current max is already greater, no need to continue.
                break;
            }

            // Update the current node's max and move up to the parent.
            current->max = newMax;
            current = current->parent;
        }
    }

    // Function to search for an interval in the tree.
    Node* SearchInterval(int low, int high)
    {
        // In case the tree is empty, throw an error.
        if (root == nullptr)
        {
            throw "Tree is empty";
        }

        // Create a target node to represent the interval being searched for.
        Node* target = new Node(low, high, nullptr);

        // Begin searching for an overlapping interval, if it exists.
        return searchInterval(root, target);
    }

    // Function that searches for the minimum overlapping interval with the 'target' in the subtree rooted at 'node'.
    Node* searchInterval(Node* node, Node* target) {
    // Check if the current node is null or if overlap is impossible and return null if so.
    if (node == nullptr || !canOverLap(node, target)) {
        return nullptr;
    }

    // Variable to keep track of the minimum overlapping interval found so far.
    Node* minimum = nullptr;

    // If the current node overlaps with the target, set it as the minimum.
    if (isOverlapping(node, target)) {
        minimum = node;
    }

    // Recursively search in the left subtree for overlapping intervals.
    Node* currentLeft = searchInterval(node->left, target);
    // Recursively search in the right subtree for overlapping intervals.
    Node* currentRight = searchInterval(node->right, target);

    // If an overlapping interval is found in the left subtree, check if it is the minimum.
    if (currentLeft != nullptr && isOverlapping(currentLeft, target)) {
        if (minimum == nullptr || currentLeft->low < minimum->low) {
            minimum = currentLeft;
        }
    }

    // If an overlapping interval is found in the right subtree, check if it is the minimum.
    if (currentRight != nullptr && isOverlapping(currentRight, target)) {
        if (minimum == nullptr || currentRight->low < minimum->low) {
            minimum = currentRight;
        }
    }

    // Return the minimum overlapping interval found.
    return minimum;
}

// Function that determines if an interval rooted at 'node' could potentially overlap with 'target'.
    bool canOverLap(Node* node, Node* target) {
        // Checks if there is no chance for overlap because 'node's interval is completely to the right of the 'target'.
        if (node->low > target->high && node->left == nullptr) {
            return false;
        }
        // Checks if the max endpoint in the 'node's left subtree is less than the 'target's low endpoint, indicating no overlap.
        if (node->max < target->low) {
            return false;
        }

        // If none of the above conditions are met, there is a potential for overlap.
        return true;
    }

    // Function that determines if two intervals overlap.
    bool isOverlapping(Node *first, Node *second) {
        // Overlap occurs if one interval's low is less than or equal to the other's high and vice versa.
        return (first->low <= second->high && second->low <= first->high);
    }
};

class IntervalTreeTester {
private:
    static const int NUM_TESTS = 3; // Constant representing the number of tests to run.
    static const int NUM_INTERVALS = 10; // Constant representing the number of intervals used in testing.

    // Function to check the result of a search operation against expected outcome.
    void checkTest(int low, int high, const char* expected, IntervalTree tree) {
        // Call the SearchInterval function of IntervalTree to find an overlapping interval.
        Node* result = tree.SearchInterval(low, high);
        cout << "Searching for [" << low << ", " << high << "], ";
        // Compare the result with expected outcome and print the comparison result.
        if (result == nullptr) {
            cout << "Result: Does not exist, Expected: " << expected << endl;
        } else {
            cout << "Result: " << result->low << ", " << result->high << " Expected: " << expected << endl;
        }
    }

    void test1()
    {
        cout << "Test 1: " << endl;

        IntervalTree tree;
        // Insert some new intervals
        tree.InsertInterval(10, 30);
        tree.InsertInterval(20, 40);
        tree.InsertInterval(0, 15);
        tree.InsertInterval(25, 35);
        tree.InsertInterval(5, 15);

        // For this example, using a simple message string to indicate an expected result
        const char *doesntExsist = "Does not exsist";

        // Perform checks for various intervals
        checkTest(22, 87, "10, 30", tree);          // Expected to overlap with interval 10-30
        checkTest(48, 59, doesntExsist, tree);      // No overlap expected
        checkTest(20, 42, "10, 30", tree);          // Expected to overlap with interval 10-30
        checkTest(33, 90, "20, 40", tree);          // Expected to overlap with interval 20-40
        checkTest(38, 92, "20, 40", tree);          // Expected to overlap with interval 20-40
        checkTest(50, 77, doesntExsist, tree);      // No overlap expected
        checkTest(13, 15, "0, 15", tree);           // Expected to overlap with interval 0-15
        checkTest(1, 17, "0, 15", tree);            // Expected to overlap with interval 0-25
        checkTest(24, 51, "10, 30", tree);          // Expected to overlap with interval 10-30
        checkTest(85, 100, doesntExsist, tree);     // No overlap expected

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
        checkTest(3, 10, "5, 25", tree);          // Expected to overlap with interval 5-25
        checkTest(36, 45, "40, 60", tree);        // No overlap expected
        checkTest(55, 65, "40, 60", tree);        // Expected to overlap with interval 40-60
        checkTest(75, 95, "70, 90", tree);        // Expected to overlap with interval 70-90
        checkTest(30, 50, "15, 35", tree);        // Expected to overlap with interval 15-35
        checkTest(42, 58, "40, 60", tree);        // Expected to overlap with interval 40-60
        checkTest(22, 23, "5, 25", tree);         // Expected to overlap with interval 5-25
        checkTest(65, 85, "50, 70", tree);        // Expected to overlap with interval 70-90
        checkTest(27, 37, "15, 35", tree);        // Expected to overlap with interval 15-35
        checkTest(10, 20, "5, 25", tree);            // Expected to overlap with interval 5-25 

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
        checkTest(0, 10, "5, 15", tree);          // Expected to overlap with interval 5-15
        checkTest(16, 19, "10, 60", tree);        // No overlap expected
        checkTest(35, 45, "10, 60", tree);        // Expected to overlap with intervals 20-50
        checkTest(56, 100, "10, 60", tree);       // Expected to overlap with interval 55-95
        checkTest(25, 30, "10, 60", tree);        // Expected to overlap with interval 20-50
        checkTest(60, 70, "10, 60", tree);        // Expected to overlap with interval 10-60
        checkTest(1, 4, doesntExist, tree);       // No overlap expected
        checkTest(96, 99, doesntExist, tree);     // No overlap expected
        checkTest(45, 75, "10, 60", tree);        // Expected to overlap with intervals 10-60
        checkTest(65, 90, "55, 95", tree);        // Expected to overlap with interval 55-95

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