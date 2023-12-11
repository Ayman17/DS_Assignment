#include <iostream>
#include <cstring>


using namespace std;

class Node 
{
    public:
        int startIndex = -1;
        int length = 0;
        int leafIndex = -1;
        Node* children = nullptr;
        Node* next = nullptr;
        
        Node(int startIndex, int length) {
            this->startIndex = startIndex;
            this->length = length;
        }

        int getBiggestMatch(char* str, int suffixIndex, int suffixLength) {
            int result = 0;
            for (int i = 0; i < length && i < suffixLength; i++) {
                if (str[suffixIndex + i] != str[startIndex + i]) {
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
    int length;
    char* str;

    SuffixTree(char *str)
    {
        this->str = str;
        length = strlen(str);
        buildTree();

        printf("Nodes: %d\n", nodeCounter);
    }

    void buildTree() {
        root = new Node(-1, 0);
        nodeCounter++;
        int length = strlen(str);
        for (int i = length - 1; i >= 0; i--)
        {
            // char* currentStr = new char[length];
            // strncpy(currentStr, str + i, length - i);
            // currentStr[length - i] = '\0';
            addSuffix(root, i);
        }
    }

    void addSuffix(Node* node, int currentIndex) {
        Node* matchingNode = nullptr;

        // int length = strlen(str);

        Node* current = node->children; 
        while (current != nullptr) {
            if (str[current->startIndex] == str[currentIndex]) {
                matchingNode = current;
                break;
            }
            current = current->next;
        }

        if (matchingNode == nullptr) {
            Node* newNode = new Node(currentIndex, length - currentIndex);
            // newNode->suffix = new char[length];
            // strncpy(newNode->suffix, str, length);
            // newNode->suffix[length] = '\0';
            node->appendChild(newNode);
            nodeCounter++;
        } else {
            int match = matchingNode->getBiggestMatch(str, currentIndex, length - currentIndex);

            if (match == matchingNode->length) {
                // char* newStr = new char[length - match];
                // strncpy(newStr, str + match, length - match);
                // newStr[length - match] = '\0';
                addSuffix(matchingNode, currentIndex + match);
            } else {
                divideNode(matchingNode, currentIndex, match);
            }
        }
    }

    void divideNode(Node* node, int currentIndex, int match) {
        Node* newNode = new Node(currentIndex + match, length - (currentIndex + match));
        nodeCounter++;
        Node* prevDataNode = new Node(node->startIndex + match, node->length - match);
        Node* prevDataChildren = node->children;
        int length = strlen(str);
        nodeCounter++;
        node->length = match;
        // printf("node: %s\n", node->suffix);
        // printf("str: %s\n", str);

        // prevDataNode->suffix = new char[length];
        // int nodeSufLength = strlen(node->suffix);
        // for (int i = 0; i < nodeSufLength; i++) {
            // prevDataNode->suffix[i] = node->suffix[i + match];
        // }
        // prevDataNode->suffix[nodeSufLength] = '\0';


        // newNode->suffix = new char[length];
        // for (int i = 0; i < length - match; i++) {
            // newNode->suffix[i] = str[i + match];
        // }
        // newNode->suffix[length - match] = '\0';

        // node->suffix[match] = '\0';
        // printf("node: %s\n", node->suffix);
        // printf("newNode: %s\n", newNode->suffix);
        // printf("prevData: %s\n\n", prevDataNode->suffix);
        node->children = nullptr;
        node->appendChild(prevDataNode);
        node->appendChild(newNode);
        prevDataNode->children = prevDataChildren;
    }

    // void Search(const char* suffix) {
    //     search(root, suffix);
    // }
    // void search(Node* node, const char* suffix) {
    //     Node* matchingNode = nullptr;
    //     Node* current = node->children;
    //     while (current != nullptr) {
    //         if (current->suffix[0] == suffix[0]) {
    //             matchingNode = current;
    //             break;
    //         }
    //     }
        
    // }
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