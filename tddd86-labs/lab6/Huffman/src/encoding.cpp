#include "encoding.h"
#include <queue>
#include "HuffmanNode.h"
#include "strlib.h"

/*
 * Builds a frequency table based on the input file
 * with the character as key and count of character as value
 */
map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    int byte = 0;
    while (true){
        byte = input.get();
        if (byte != -1){
            freqTable[byte]++; // ska ++ vara först?
        } else {    // om det är end of file
            freqTable[PSEUDO_EOF]++;
            break;
        }
    }
    return freqTable;
}

/*
 * Builds an Huffman encoding tree based on the frequency table for the file input
 */
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode> huffManQueue;

    for(auto elem : freqTable){
        huffManQueue.push(HuffmanNode{elem.first,elem.second});
    }

    while(huffManQueue.size() != 1){
        HuffmanNode* firstNode = new HuffmanNode{huffManQueue.top()};
        huffManQueue.pop();
        HuffmanNode* secondNode = new HuffmanNode{huffManQueue.top()};
        huffManQueue.pop();
        int sum = firstNode->count+secondNode->count;
        huffManQueue.push(HuffmanNode{NOT_A_CHAR,sum,firstNode,secondNode});
    }
    return new HuffmanNode{huffManQueue.top()};
}

/*
 * Help function for buildEncodingMap that maps character to it's path in the huffman Tree
 */
void treeHelper(map<int,string>& encodingMap, HuffmanNode* currentPoint, string charPath =""){
    if(currentPoint == nullptr){
        return;
    }
    if(currentPoint->character != NOT_A_CHAR){
        encodingMap[currentPoint->character] = charPath;
    }
    treeHelper(encodingMap,currentPoint->one, charPath+"1");
    treeHelper(encodingMap, currentPoint->zero,charPath+"0");

}

/*
 * Function that uses treeHelper to build the Encoding map for the input tree
 */
map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;
    treeHelper(encodingMap,encodingTree);
    return encodingMap;
}

/*
 * Transforms a file to binary code using the encoding map for the file.
 */
void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {

    bool shouldRun = true;
    while(shouldRun){
        int byte = 0;
        byte = input.get();
        string bytePath;
        if(byte == -1){
            bytePath = encodingMap.at(PSEUDO_EOF);
            shouldRun = false;

        }else{
            bytePath = encodingMap.at(byte);
        }
        for(auto b:bytePath){
            if(b == '0'){
                output.writeBit(0);
            }else if (b == '1'){
                output.writeBit(1);
            }
        }
    }
}

/*
 * Transforms a binary code back to the original file
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* current = encodingTree;
    while(true){

        if(current->character == PSEUDO_EOF){
            break;
        }
        else if(current->character != NOT_A_CHAR){
            output.put((char)current->character);
            current = encodingTree;
        }else{
            int bit = input.readBit();
            if(bit == 0){
                current = current->zero;
            }else{
                current = current->one;
            }
        }
    }
}

/*
 * Writes a header to the compressed file, and then compresses the file
 */
void compress(istream& input, obitstream& output) {
    map<int, int> header = buildFrequencyTable(input);

    output << '{';

    for (auto const &elem : header){
        string first = to_string(elem.first);
        for (auto const &c : first){
            output << c;
        }
        output << ':';
        string second = to_string(elem.second);
        for (auto const &c : second){
            output << c;
        }
        if (elem.first != header.rbegin()->first){
            output << ',';
        }
    }

    output << '}';
    output << '\n';
    input.clear();
    input.seekg(0, input.beg);  //clears input so we can use it in encoding
    HuffmanNode* encodingTree = buildEncodingTree(header);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * Gets the header and decodes the compressed file
 */
void decompress(ibitstream& input, ostream& output) {
    string header;
    map<int, int> freqTable;
    string strChar;
    string strCount;
    int character;
    int count;

    getline(input, header);
    header.erase(header.begin());
    while (!header.empty()){
        strChar = header.substr(0, header.find_first_of(':'));
        if (header.find_first_of(',') != string::npos){
            strCount = header.substr(header.find_first_of(':')+1, header.find_first_of(','));
            header.erase(0, header.find_first_of(',')+1);
        } else { // inget kommatecken kvar -> är på sista elementet
            strCount = header.substr(header.find_first_of(':')+1, header.find_first_of('}'));
            header.erase(0, header.find_first_of('}')+1);
        }

        character = stoi(strChar);
        count = stoi(strCount);
        freqTable[character] = count;
    }
    HuffmanNode* tree = buildEncodingTree(freqTable);
    decodeData(input, tree, output);
    freeTree(tree);
}

/*
 * Frees the allocated memory of the input tree
 */
void freeTree(HuffmanNode* node) {
    if (node == nullptr){
        return;
    } else if (node->isLeaf()){
        delete node;
    } else {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
