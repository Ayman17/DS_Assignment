#include <iostream>
#include <cstring>


using namespace std;

class Node 
{
    public:
        char* suffix;
        int startIndex;
        int leafIndex;
        Node* children = nullptr;
        Node* next = nullptr;
};

class SuffixTree
{
    public:
        Node* root = nullptr;

    SuffixTree(char*  s)
    {
        root = new Node();
        for (int i = strlen(s) - 1 ; i >= 0; i--)
        {

        }
    }
};

int main()
{
    // Construct a suffix tree containing all suffixes of "bananabanaba$"

    //            0123456789012
    SuffixTree t("bananabanaba$");

    t.Search("ana"); // Prints: 1 3 7
    t.Search("naba"); // Prints: 4 8
    
    // Add test cases here.

    return 0;
}