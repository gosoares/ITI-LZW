#include <iostream>
#include <fstream>
#include <map>
#include "BitWriter.h"
#include "CompressTree.h"

using namespace std;

int main() {

    clock_t initialTime = clock(), finalTime;

    string fileName = "teste.txt";
    unsigned long dictionarySizeLimit = 999999;

    CompressTree compressTree(dictionarySizeLimit);

    ifstream inFile(fileName, ifstream::binary);
    BitWriter writer(fileName + ".LZW");

    if(!inFile.good()) {
        cout << "Failed to open the file!" << endl;
    }

    Node* current = compressTree.root;
    int symbol;
    while((symbol = inFile.get()) != EOF) {
        Node* child = current->getChild(static_cast<unsigned char>(symbol));
        if(child == nullptr) { // nó atual não tem esse byte, então deve comprimi-lo, e inseri-lo
            writer.writeIndex(current->index, compressTree.bitsForIndex);
            compressTree.newNodeChild(current, symbol);
            inFile.unget(); // retorna ultimo byte lido, para fazer o matching
            current = compressTree.root;
        } else {
            current = child;
        }
    }

    if(current != compressTree.root) {
        writer.writeIndex(current->index, compressTree.bitsForIndex);
    }

    writer.writeIndex(compressTree.root->getChild(EOF)->index, compressTree.bitsForIndex);

    inFile.close();
    writer.close();

    finalTime = clock();
    long executionTime = ((finalTime - initialTime) / (CLOCKS_PER_SEC / 1000));
    cout << "t: " << executionTime << " ms" << endl;

    return 0;
}