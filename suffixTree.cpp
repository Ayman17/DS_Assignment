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
        
        Node(int startIndex, int length) {
            this->startIndex = startIndex;
            this->length = length;
        }

        int getBiggestMatch(char* str, int otherSuffixIndex, int otherSuffixLength) {
            int result = 0;
            for (int i = 0; i < length && i < otherSuffixLength; i++) {
                if (str[otherSuffixIndex + i] != str[startIndex + i]) {
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
            addSuffix(root, i, i);
        }
    }

    void addSuffix(Node* node, int currentIndex, int suffixIndex) {
        Node* matchingNode = nullptr;

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
            newNode->suffixIndex = suffixIndex;
            node->appendChild(newNode);
            nodeCounter++;
        } else {
            int match = matchingNode->getBiggestMatch(str, currentIndex, length - currentIndex);

            if (match == matchingNode->length) {
                addSuffix(matchingNode, currentIndex + match, suffixIndex);
            } else {
                divideNode(matchingNode, currentIndex, match, suffixIndex);
            }
        }
    }

    void divideNode(Node* node, int currentIndex, int match, int newSuffixIndex) {
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