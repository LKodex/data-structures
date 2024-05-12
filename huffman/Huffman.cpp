#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <queue>
#include "BitBuffer.hpp"

void compress(FILE *inputFile, FILE *outputFile);
void decompress(FILE *inputFile, FILE *outputFile);
void getPreorderCharacters(class HuffmanTreeNode *tree, std::vector<uint8_t> *vector);
void getPreorderTreeBits(class HuffmanTreeNode *tree, BitBuffer *bitBuffer);
void getCharacterTable(class HuffmanTreeNode *tree, std::unordered_map<uint8_t, class Node> *table, int quantity, uint8_t bits);
void printHuffmanTree(class HuffmanTreeNode *tree);

class Node
{
    public:
        Node();
        Node(int bitQuantity, uint8_t bits);
        ~Node();
        int bitQuantity;
        uint8_t bits;
};

Node::Node()
    : bitQuantity(0)
    , bits(0b0)
{ }

Node::Node(int bitQuantity, uint8_t bits)
    : bitQuantity(bitQuantity)
    , bits(bits)
{ }

Node::~Node()
{ }

class HuffmanTreeNode
{
    public:
        HuffmanTreeNode();
        HuffmanTreeNode(uint8_t byte, uint32_t frequency);
        ~HuffmanTreeNode();
        uint8_t byte;
        uint32_t frequency;
        HuffmanTreeNode *left;
        HuffmanTreeNode *right;
};

HuffmanTreeNode::HuffmanTreeNode()
    : byte(0)
    , frequency(0)
    , left(nullptr)
    , right(nullptr)
{ }

HuffmanTreeNode::HuffmanTreeNode(uint8_t byte, uint32_t frequency)
    : byte(byte)
    , frequency(frequency)
    , left(nullptr)
    , right(nullptr)
{ }

HuffmanTreeNode::~HuffmanTreeNode()
{ }

class MinHeapNodeComparator
{
    public:
        bool operator() (HuffmanTreeNode *&left, HuffmanTreeNode *&right) const
        {
            return left->frequency > right->frequency;
        };
};

int main(int argc, char *argv[])
{
    bool isUsageIncorrect = argc != 4;
    if (isUsageIncorrect)
    {
        std::string programName = argv[0];
        std::string correctUsageMessage("Usage: " + programName + " <operation type> " + " <input file> " + " <output file> ");
        std::cout << correctUsageMessage << std::endl;
        return EXIT_SUCCESS;
    }

    char * operationArg = argv[1];
    char * inputFileNameArg = argv[2];
    char * outputFileNameArg = argv[3];

    char operation = operationArg[0];

    bool isValidOperation = operation == 'c' || operation == 'd';
    if (!isValidOperation)
    {
        std::string errorMessage("[ERR] Invalid operation type\n - c -> Compress the input file into the output file\n - d -> Decompress de input file into the output file");
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    FILE *inputFile = fopen(inputFileNameArg, "r");
    FILE *outputFile = fopen(outputFileNameArg, "w");

    bool isInputFileOpen = inputFile != nullptr;
    if (!isInputFileOpen)
    {
        std::string errorMessage("[ERR] Failed to open the input file " + std::string(inputFileNameArg));
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    bool isOutputFileOpen = outputFile != nullptr;
    if (!isOutputFileOpen)
    {
        std::string errorMessage("[ERR] Failed to open the output file " + std::string(outputFileNameArg));
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    bool isCompressChoice = operation == 'c';
    if (isCompressChoice)
    {
        compress(inputFile, outputFile);
    }

    bool isDecompressChoice = operation == 'd';
    if (isDecompressChoice)
    {
        decompress(inputFile, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}

void compress(FILE *inputFile, FILE *outputFile)
{
    std::unordered_map<uint8_t, uint32_t> charactersFrequency;
    while (true)
    {
        uint8_t byte;
        fread(&byte, 1, 1, inputFile);

        if (feof(inputFile) != 0)
        {
            break;
        }
        
        charactersFrequency[byte]++;
    }
    
    rewind(inputFile);
    
    uint16_t distinctCharacterCount = charactersFrequency.size();
    
    std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapNodeComparator> nodeFrequencyQueue;

    for (auto it = charactersFrequency.begin(); it != charactersFrequency.end(); ++it)
    {
        uint8_t byte = it->first;
        uint32_t frequency = it->second;
        HuffmanTreeNode * characterNode = new HuffmanTreeNode(byte, frequency);

        nodeFrequencyQueue.push(characterNode);
    }

    while (nodeFrequencyQueue.size() > 1)
    {

        HuffmanTreeNode *left = nodeFrequencyQueue.top();
        nodeFrequencyQueue.pop();

        HuffmanTreeNode *right = nodeFrequencyQueue.top();
        nodeFrequencyQueue.pop();
        
        HuffmanTreeNode *internalNode = new HuffmanTreeNode();
        internalNode->frequency = left->frequency + right->frequency;
        internalNode->left = left;
        internalNode->right = right;

        nodeFrequencyQueue.push(internalNode);
    }

    HuffmanTreeNode *huffmanTree = nodeFrequencyQueue.top();
    nodeFrequencyQueue.pop();
    // printHuffmanTree(huffmanTree);
    uint32_t totalCharacterCount = huffmanTree->frequency;

    fwrite(&distinctCharacterCount, sizeof(uint16_t), 1, outputFile);
    fwrite(&totalCharacterCount, sizeof(uint32_t), 1, outputFile);

    std::vector<uint8_t> vector;
    getPreorderCharacters(huffmanTree, &vector);
    for (auto it = vector.begin(); it != vector.end(); ++it)
    {
        fwrite(&*it, sizeof(uint8_t), 1, outputFile);
    }

    BitBuffer outputBitBuffer(outputFile);
    getPreorderTreeBits(huffmanTree, &outputBitBuffer);

    std::unordered_map<uint8_t, Node> characterTable;
    getCharacterTable(huffmanTree, &characterTable, 0, 0b0);

    while (true)
    {
        uint8_t byte;
        fread(&byte, 1, 1, inputFile);

        if (feof(inputFile) != 0)
        {
            break;
        }

        Node node = characterTable[byte];
        for (int i = 0; i < node.bitQuantity; i++)
        {
            int bitShift = node.bitQuantity - i - 1;
            uint8_t bit = node.bits >> bitShift;
            outputBitBuffer.put_bit(bit);
        }
    }
    outputBitBuffer.write_byte();
}

void printHuffmanTree(class HuffmanTreeNode *tree)
{
    if (tree == nullptr)
    {
        return;
    }
    std::cout << tree->byte << ": " << tree->frequency << std::endl;
    printHuffmanTree(tree->left);
    printHuffmanTree(tree->right);
}

void getPreorderCharacters(class HuffmanTreeNode *tree, std::vector<uint8_t> *vector)
{

    bool isNullNode = tree == nullptr;
    if (isNullNode)
    {
        return;
    }

    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    if (isLeaf)
    {
        vector->push_back(tree->byte);
    }
    getPreorderCharacters(tree->left, vector);
    getPreorderCharacters(tree->right, vector);
}

void getPreorderTreeBits(class HuffmanTreeNode *tree, BitBuffer *bitBuffer)
{
    bool isNullNode = tree == nullptr;
    if (isNullNode)
    {
        return;
    }
    
    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    uint8_t bit;
    if (isLeaf)
    {
        bit = 0b1;
    }
    else
    {
        bit = 0b0;
    }
    bitBuffer->put_bit(bit);

    getPreorderTreeBits(tree->left, bitBuffer);
    getPreorderTreeBits(tree->right, bitBuffer);
}

void getCharacterTable(class HuffmanTreeNode *tree, std::unordered_map<uint8_t, Node> *table, int quantity, uint8_t bits)
{
    bool isNullNode = tree == nullptr;
    if (isNullNode)
    {
        return;
    }

    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    if (isLeaf)
    {
        Node node(quantity, bits);
        (*table)[tree->byte] = node;
    }
    bits <<= 1;
    getCharacterTable(tree->left, table, quantity + 1, bits | 0b0);
    getCharacterTable(tree->right, table, quantity + 1, bits | 0b1);
}

void decompress(FILE *inputFile, FILE *outputFile)
{

}
