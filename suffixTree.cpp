#include <iostream>
#include <cstring>


using namespace std;

class Node 
{
    public:
        int startIndex = -1;
        int length = 0;
        int suffixIndex = -1;
        Node* children = nullptr;
        Node* next = nullptr;
        
        Node(int startIndex, int length)
        {
            this->startIndex = startIndex;
            this->length = length;
        }

        // get the length of the longest common prefix between the current node and the given node
        int getBiggestMatch(char* str, int otherSuffixIndex, int otherSuffixLength) 
        {
            int result = 0;
            for (int i = 0; i < length; i++) 
            {
                if (i < otherSuffixLength)
                {
                    break;   
                }
                if (str[otherSuffixIndex + i] != str[startIndex + i]) 
                {
                    return result;
                }
                result++;
            }
        }

        // add a child node to the children linked list
        void appendChild(Node* node) 
        {
            if (children == nullptr) 
            {
                children = node; 
            } else 
            {
                Node* prev = children;
                while (prev->next != nullptr) 
                {
                    prev = prev->next;
                }
                prev->next = node;
            }
        }
};

class SuffixTree
{
public:
    Node *root = nullptr;
    int length;
    char* str; // pointer for the original string

    SuffixTree(const char* str)
    {
        buildTree(str);
    }

    SuffixTree() = default;

    // construct suffix tree from the string
    void buildTree(const char* str)
    {
        // create a copy of the original string
        this->str = new char[strlen(str) + 1];
        strcpy(this->str, str);
        this->str[strlen(str)] = '\0';

        length = strlen(str); // save the length of the string

        // create the root node of the tree with empty suffix 
        root = new Node(-1, 0);

        for (int i = length - 1; i >= 0; i--)
        {
            // add suffixes to the tree
            addSuffix(root, i, i);
        }
    }

    void addSuffix(Node* node, int currentIndex, int suffixIndex) 
    {
        Node* matchingNode = nullptr; // node that matches the first characters of the suffix

        Node* current = node->children; // start from the first child of the node
        while (current != nullptr) 
        {
            // check if the current node matches the first characters of the suffix
            if (str[current->startIndex] == str[currentIndex]) 
            {
                matchingNode = current;
                break;
            }
            current = current->next; // move to the next child
        }

        // if no matching node is found, create a new node and add it as a child of the node
        if (matchingNode == nullptr) 
        {
            Node* newNode = new Node(currentIndex, length - currentIndex);
            newNode->suffixIndex = suffixIndex;
            node->appendChild(newNode);
        } else 
        {
            // if a matching node is found, get the length of the match
            int match = matchingNode->getBiggestMatch(str, currentIndex, length - currentIndex);

            // if the match is the same length as the suffix, move one level down
            if (match == matchingNode->length) 
            {
                addSuffix(matchingNode, currentIndex + match, suffixIndex);
            } else 
            {
                // if the match is shorter than the suffix, divide the node into parent and tow children
                divideNode(matchingNode, currentIndex, match, suffixIndex);
            }
        }
    }

    void divideNode(Node* node, int currentIndex, int match, int newSuffixIndex) 
    {
        // create a new node starts after the match until the end of the string
        Node* newNode = new Node(currentIndex + match, length - (currentIndex + match));
        newNode->suffixIndex = newSuffixIndex;  
        // rest of the old node is moved to a new node with the unmatched chars
        Node* prevDataNode = new Node(node->startIndex + match, node->length - match);

        prevDataNode->suffixIndex = node->suffixIndex;  
        prevDataNode->children = node->children;  // move the old node children to the new node

        // update the old node
        node->length = match;
        node->children = nullptr;
        node->suffixIndex = -1; // old node is no longer a leaf

        // append children to the old node
        node->appendChild(prevDataNode);
        node->appendChild(newNode);
    }

    void Search(const char* suffix) 
    {
        search(root, suffix); // handel search recursivly 
    }

    void search(Node* node, const char* suffix) 
    {
        int i = 0;
        int suffixLength = strlen(suffix); // save the length of the suffix

        // loop unitl we reach the end of the node or the suffix
        for (i = 0; i < node->length; i++) 
        {
            if (i < suffixLength)
            {
                break;
            }
            // check if the current character matches the current character in the suffix
            if (str[node->startIndex + i] != suffix[i]) 
            {
                printf("No match found");
                return;
            }
        }

        // if we reached the end of the suffix, print all the leaves indexes
        if (i == suffixLength) 
        {
            printAllLeavesIndexes(node);
            return;
        }

        // loop until we find a child node that matches the next character in the suffix
        Node* current = node->children; 
        while (current != nullptr) 
        {
            if (str[current->startIndex] == suffix[i]) 
            {
                char* newSuffix = new char[suffixLength - i];
                strncpy(newSuffix, suffix + i, suffixLength - i);
                newSuffix[suffixLength - i] = '\0';

                // recall search with the new node and suffix
                search(current, newSuffix);
                return;
            }
            current = current->next;
        }

        printf("No match found");
    }

    ~SuffixTree() 
    {
        delete[] str;
        deleteNodes(root);
    }

    void deleteNodes(Node* node) 
    {
        if (node->children != nullptr) 
        {
            deleteNodes(node->children);
        }

        if (node->next != nullptr) 
        {
            deleteNodes(node->next);
        }
        delete node;
    }




    // if the node is a leaf, print its index and move to the next node and child node
    void printLeaves(Node* node) 
    {
        if (node->suffixIndex != -1) 
        { 
            printf("%d ", node->suffixIndex);
        }

        if (node->children != nullptr) 
        {
            printLeaves(node->children);
        }

        if (node->next != nullptr) 
        {
            printLeaves(node->next);
        }
    }

    // the setup function for print leaves 
    void printAllLeavesIndexes(Node* node) 
    {
        if (node->suffixIndex != -1) 
        { 
            printf("%d ", node->suffixIndex);
        }

        if (node->children != nullptr) 
        {
            printLeaves(node->children);
        }
    }
};

class TestSuffixTree 
{
    SuffixTree tree;

public: 
    TestSuffixTree(const char* str) 
    {
        tree.buildTree(str);
    }

    void test(char* suffix, char* expected) 
    {
        printf("Result: ");
        tree.Search(suffix); 
        printf(", Expected: %s\n", expected);
    }
};

int main()
{
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTree t("bananabanaba$");
    t.Search("ana"); // Prints: 1 3 7
    printf("\n");
    t.Search("naba"); // Prints: 4 8
    printf("\n\n");

    // Add test cases here.
    printf("Test1 : bananabanaba$\n");
    TestSuffixTree t1("bananabanaba$");
    t1.test("ana", "7 3 1");
    t1.test("naba", "8 4");
    t1.test("bana", "6 0");
    t1.test("ban", "6 0");

    printf("\nTest2 : suffixtree$\n");
    TestSuffixTree t2("suffixtree$");
    t2.test("tree", "6");
    t2.test("ix", "4");
    t2.test("f", "3 2");

    printf("\nTest3 : hamhamhammamahamahm$\n");
    TestSuffixTree t3("hamhamhammamahamahm$");
    t3.test("ham", "13 6 3 0");
    t3.test("mam", "9");
    t3.test("z", "No match found");
    t3.test("hamj", "No match found");

    return 0;
}
