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

        int getBiggestMatch(char* str, int otherSuffixIndex, int otherSuffixLength) 
        {
            int result = 0;
            for (int i = 0; i < length && i < otherSuffixLength; i++) 
            {
                if (str[otherSuffixIndex + i] != str[startIndex + i]) 
                {
                    return result;
                }
                result++;
            }
        }

        void appendChild(Node* node) 
        {
            if (children == nullptr) 
            {
                children = node; 
            } else {
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
    int nodeCounter = 0;
    int length;
    char* str;

    SuffixTree(const char* str)
    {
        buildTree(str);
    }

    SuffixTree() = default;

    void buildTree(const char* str) 
    {
        this->str = new char[strlen(str) + 1];
        strcpy(this->str, str);
        this->str[strlen(str)] = '\0';

        length = strlen(str);

        root = new Node(-1, 0);
        nodeCounter++;
        int length = strlen(str);
        for (int i = length - 1; i >= 0; i--)
        {
            addSuffix(root, i, i);
        }
    }

    void addSuffix(Node* node, int currentIndex, int suffixIndex) 
    {
        Node* matchingNode = nullptr;

        Node* current = node->children; 
        while (current != nullptr) {
            if (str[current->startIndex] == str[currentIndex]) 
            {
                matchingNode = current;
                break;
            }
            current = current->next;
        }

        if (matchingNode == nullptr) 
        {
            Node* newNode = new Node(currentIndex, length - currentIndex);
            newNode->suffixIndex = suffixIndex;
            node->appendChild(newNode);
            nodeCounter++;
        } else 
        {
            int match = matchingNode->getBiggestMatch(str, currentIndex, length - currentIndex);

            if (match == matchingNode->length) 
            {
                addSuffix(matchingNode, currentIndex + match, suffixIndex);
            } else {
                divideNode(matchingNode, currentIndex, match, suffixIndex);
            }
        }
    }

    void divideNode(Node* node, int currentIndex, int match, int newSuffixIndex) 
    {
        Node* newNode = new Node(currentIndex + match, length - (currentIndex + match));
        newNode->suffixIndex = newSuffixIndex;  
        nodeCounter++;
        Node* prevDataNode = new Node(node->startIndex + match, node->length - match);
        // Node* prevDataChildren = node->children;
        nodeCounter++;

        prevDataNode->suffixIndex = node->suffixIndex;
        prevDataNode->children = node->children;

        node->length = match;
        node->children = nullptr;
        node->suffixIndex = -1;
        node->appendChild(prevDataNode);
        node->appendChild(newNode);
    }

    void Search(const char* suffix) 
    {
        search(root, suffix);
    }

    void search(Node* node, const char* suffix) 
    {
        int i = 0;
        int suffixLength = strlen(suffix);

        for (i = 0; i < node->length && i < suffixLength; i++) 
        {
            if (str[node->startIndex + i] != suffix[i]) {
                printf("No match found");
                return;
            }
        }


        if (i == suffixLength) 
        {
            printAllLeavesIndexes(node);
            // printf("\n");
            return;
        }

        Node* current = node->children; 
        while (current != nullptr) {
            if (str[current->startIndex] == suffix[i]) 
            {
                char* newSuffix = new char[suffixLength - i];
                strncpy(newSuffix, suffix + i, suffixLength - i);
                newSuffix[suffixLength - i] = '\0';

                search(current, newSuffix);
                return;
            }
            current = current->next;
        }

        printf("No match found");
    }

    void printLeaves(Node* node) {
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

class TestSuffixTree {
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
    // t.Search("ana"); // Prints: 1 3 7
    // t.Search("naba"); // Prints: 4 8
    
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
