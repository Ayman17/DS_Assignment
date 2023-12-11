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
            int length = strlen(str);
            for (int i = 0; i < length; i++) {
                if (str[i] != suffix[i]) {
                    return result;
                }
                result++;
            }
        }

        void appendChild(Node* node) {
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
        int length = strlen(str);
        for (int i = length - 1; i >= 0; i--)
        {
            
            char* currentStr = new char[length];
            strncpy(currentStr, str + i, length - i);
            currentStr[length - i] = '\0';
            addSuffix(root, currentStr);
        }
    }

    void addSuffix(Node* node, const char* str) {
        Node* matchingNode = nullptr;

        int length = strlen(str);

        Node* current = node->children; 
        while (current != nullptr) {
            if (current->suffix[0] == str[0]) {
                matchingNode = current;
                break;
            }
            current = current->next;
        }

        if (matchingNode == nullptr) {
            Node* newNode = new Node();
            newNode->suffix = new char[length];
            strncpy(newNode->suffix, str, length);
            newNode->suffix[length] = '\0';
            node->appendChild(newNode);
            nodeCounter++;
        } else {
            int match = matchingNode->getBiggestMatch(str);

            if (match == strlen(matchingNode->suffix)) {
                char* newStr = new char[length - match];
                strncpy(newStr, str + match, length - match);
                newStr[length - match] = '\0';
                addSuffix(matchingNode, newStr);
            } else {
                divideNode(matchingNode, str, match);
            }
        }
    }

    void divideNode(Node* node, const char* str, int match) {
        Node* newNode = new Node();
        nodeCounter++;
        Node* prevDataNode = new Node();
        Node* prevDataChildren = node->children;
        int length = strlen(str);
        nodeCounter++;
        // printf("node: %s\n", node->suffix);
        // printf("str: %s\n", str);

        prevDataNode->suffix = new char[length];
        int nodeSufLength = strlen(node->suffix);
        for (int i = 0; i < nodeSufLength; i++) {
            prevDataNode->suffix[i] = node->suffix[i + match];
        }
        prevDataNode->suffix[nodeSufLength] = '\0';


        newNode->suffix = new char[length];
        for (int i = 0; i < length - match; i++) {
            newNode->suffix[i] = str[i + match];
        }
        newNode->suffix[length - match] = '\0';

        node->suffix[match] = '\0';
        // printf("node: %s\n", node->suffix);
        // printf("newNode: %s\n", newNode->suffix);
        // printf("prevData: %s\n\n", prevDataNode->suffix);
        node->children = nullptr;
        node->appendChild(prevDataNode);
        node->appendChild(newNode);
        prevDataNode->children = prevDataChildren;
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