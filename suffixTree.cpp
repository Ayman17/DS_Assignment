#include <iostream>
#include <cstring>


using namespace std;

class Node 
{
    public:
        char* suffix = nullptr;
        int startIndex = -1;
        int leafIndex = -1;
        Node* children = nullptr;
        Node* next = nullptr;

        int getBiggestMatch(const char* str) {
            int result = 0;
            for (int i = 0; i < strlen(str); i++) {
                if (str[i] != suffix[i]) {
                    return result;
                }
                result++;
            }
        }

        void appendNode(Node* node) {
            if (children == nullptr) {
                children = node; 
            } else {
                Node* prev = children;
                while (prev->next != nullptr) {
                    prev = prev->next;
                }
                prev->next = node;
                // printf("main: %s\n", suffix);
                // printf("Prev: %s\n", prev->suffix);
                // printf("Node: %s\n", node->suffix);
                // printf("wtf?: %s\n\n", prev->next->suffix);
            }
        }
};

class SuffixTree
{
public:
    Node *root = nullptr;
    int nodeCounter = 0;

    SuffixTree(char *str)
    {
        buildTree(str);
        
        printAll(root);
        printf("Nodes: %d\n", nodeCounter);
    }

    void buildTree(const char* str) {
        root = new Node();
        nodeCounter++;
        Node* last = nullptr;
        int length = strlen(str);
        for (int i = length - 1; i >= 0; i--)
        {
            char* currentStr = new char[length];
            strncpy(currentStr, str + i, length - i);
            currentStr[length - i] = '\0';

            Node* current = root->children;
            bool foundMatch = false;
            while (current != nullptr) {
                int match = current->getBiggestMatch(currentStr);
                if (match != 0) {
                    divideNode(current, currentStr, match);
                    foundMatch = true;
                    break;
                }
                current = current->next;
            }

            if (foundMatch) {
                continue;
            }

            Node* newNode = new Node();
            nodeCounter++;

            newNode->suffix = currentStr;

            if (last != nullptr) { 
                last->next = newNode;
            }

            if (root->children == nullptr) {
                root->children = newNode;
            }

            last = newNode;
        }
    }

    void printAll(Node* node) {
        if (node == nullptr) {
            return;
        }

        Node* current = node->children;
        while (current != nullptr) {
            if (current->suffix != nullptr) {
                printf("%s\n", current->suffix);
            }
            printAll(current->children);
            current = current->next;
        }

    }

    void divideNode(Node* node, const char* str, int match) {
        Node* newNode = new Node();
        nodeCounter++;
        Node* prevDataNode = new Node();
        nodeCounter++;
        // printf("node: %s\n", node->suffix);
        // printf("str: %s\n", str);

        prevDataNode->suffix = new char[strlen(str)];
        for (int i = 0; i < strlen(node->suffix); i++) {
            prevDataNode->suffix[i] = node->suffix[i + match];
        }
        prevDataNode->suffix[strlen(node->suffix)] = '\0';


        newNode->suffix = new char[strlen(str)];
        for (int i = 0; i < strlen(str) - match; i++) {
            newNode->suffix[i] = str[i + match];
        }
        newNode->suffix[strlen(str) - match] = '\0';

        node->suffix[match] = '\0';
        // printf("node: %s\n", node->suffix);
        // printf("newNode: %s\n", newNode->suffix);
        // printf("prevData: %s\n", prevDataNode->suffix);
        node->appendNode(prevDataNode);
        node->appendNode(newNode);
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

    return 0;
}