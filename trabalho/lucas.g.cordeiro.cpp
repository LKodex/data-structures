//////////////////////////////////////////////////////
//                                                  //
//          Autor: Lucas Gonçalves Cordeiro         //
//          TRABALHO DE ESTRUTURA DE DADOS          //
//          Professor: Diego Padilha Rubert         //
//                                                  //
//////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <queue>

void compress(FILE *inputFile, FILE *outputFile);
std::unordered_map<uint8_t, uint32_t> countCharactersFrequency(FILE *file);
std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> createCharactersMinHeap(std::unordered_map<uint8_t, uint32_t> charactersFrequencyTable);
class HuffmanTreeNode * buildHuffmanTree(std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> priorityQueue);
void writeDistinctCharacterQuantityHeader(FILE *file, uint16_t quantity);
void writeTotalCharacterQuantityHeader(FILE *file, uint32_t quantity);
std::vector<uint8_t> getPreorderCharacters(class HuffmanTreeNode *root);
void _getPreorderCharacters(class HuffmanTreeNode *tree, std::vector<uint8_t> *vector);
void writeCharacters(FILE *file, std::vector<uint8_t> characters);
void writePreorderTreeHeader(class HuffmanTreeNode *tree, class BitBuffer *bitBuffer);
std::unordered_map<uint8_t, class Node> createCharacterBitTable(class HuffmanTreeNode *root);
void _createCharacterBitTable(class HuffmanTreeNode *tree, std::unordered_map<uint8_t, class Node> *table, int quantity, uint8_t bits);
void compressInputFile(FILE *file, BitBuffer *bitWritter, std::unordered_map<uint8_t, Node> characterTable);

void decompress(FILE *inputFile, FILE *outputFile);
uint16_t readDistinctCharactersHeader(FILE *file);
uint32_t readTotalCharactersHeader(FILE *file);
std::vector<uint8_t> readCharactersHeader(FILE *file, uint16_t characterCount);
class HuffmanTreeNode *readTreeHeaderAndRebuildHuffmanTree(std::vector<uint8_t> characters, class BitBuffer *bitBuffer);
class HuffmanTreeNode *_readTreeHeaderAndRebuildHuffmanTree(std::vector<uint8_t> *characters, class BitBuffer *bitBuffer, int *characterIndex);
std::unordered_map<class Node, uint8_t> createCharacterTableForDecompression(class HuffmanTreeNode *root);
void _createCharacterTableForDecompression(class HuffmanTreeNode *tree, std::unordered_map<class Node, uint8_t> *table, int quantity, uint8_t bits);
void translateAndWriteCharacters(FILE *outputFile, class BitBuffer *bitReader, std::unordered_map<class Node, uint8_t> characterTable, uint32_t totalCharacters);

class BitBuffer;
class Node;
class HuffmanTreeNode;
class MinHeapHuffmanTreeNodeComparator;

/**
 * Class to read and write files bit per bit
*/
class BitBuffer {
	public:
		BitBuffer(FILE *file);
		uint8_t free_bits();

		// Write mode
		void put_bit(uint8_t bit);
		void write_byte();

		// Read mode
		uint8_t get_bit();

	private:
		FILE *file;
		uint8_t buffer;
		uint8_t size;
};
  
BitBuffer::BitBuffer(FILE *file)
    : file(file)
{ }

/**
 * Append a bit to the buffer, if the buffer fills up the 8 bits then it calls write_byte()
 * @param bit a byte containing the desired bit at the least significant bit
*/
void BitBuffer::put_bit(uint8_t bit)
{
	uint8_t new_bit = bit & 1;
	buffer <<= 1;
	buffer |= new_bit;
	size++;
	if (size == 8) write_byte();
}

/**
 * Write the buffer into the file, if there is less than 8 bits it is shifted to the left until
 * the first bit appended reach the most significant bit
*/
void BitBuffer::write_byte()
{
	bool isBufferEmpty = size == 0;
	if (isBufferEmpty)
	{
		return;
	}

	buffer <<= free_bits();
	fwrite(&buffer, sizeof(uint8_t), 1, file);
	size = 0;
	buffer = 0;
}

/**
 * Return the next most significant bit of the buffer (the bit at the left). If the buffer is
 * empty then it reads another byte from the file. If the file is empty then it returns 0b10.
 * If an error occurred trying to read the file then it returns 0b11
 * @return the next most significant bit of the buffer
*/
uint8_t BitBuffer::get_bit()
{
	bool isBufferEmpty = size == 0;
	if (isBufferEmpty)
	{
		fread(&buffer, 1, 1, file);
		size = 8;
	}

	bool isEOF = feof(file) != 0;
	if (isEOF)
	{
		const uint8_t _EOF = 2; // end of file return value
		return _EOF;
	}

	bool isReadError = ferror(file) != 0;
	if (isReadError)
	{
		const uint8_t _ERROR = 3; // error return value
		return _ERROR;
	}

	uint8_t read_bit = (buffer >> --size) & 1;
	return read_bit;
}

/**
 * Returns how many bits is unused
 * @return quantity of unused bits on buffer
*/
uint8_t BitBuffer::free_bits()
{
	return 8 - size;
}

/**
 * Class for mapping the quantity of bits and the bits of compressed characters
*/
class Node
{
    public:
        Node();
        Node(int bitQuantity, uint8_t bits);
        ~Node();
        int bitQuantity;
        uint8_t bits;
        bool operator==(const Node &other);
};


template<>
struct std::equal_to<Node>
{
    bool operator() (const Node &lhs, const Node &rhs) const
    {
        return lhs.bitQuantity == rhs.bitQuantity && lhs.bits == rhs.bits;       
    };
};

template<>
struct std::hash<Node>
{
    size_t operator()(const Node &node) const
    {
        return (std::hash<int>()(node.bitQuantity) << 1)
            ^ (std::hash<uint8_t>()(node.bits) >> 1);
    }
};

Node::Node()
    : bitQuantity(0)
    , bits(0)
{ }

bool Node::operator==(const Node &other)
{
    return bitQuantity == other.bitQuantity && bits == other.bits;
}

Node::Node(int bitQuantity, uint8_t bits)
    : bitQuantity(bitQuantity)
    , bits(bits)
{ }

Node::~Node()
{ }

/**
 * Class for building an Huffman tree storing a character and its respective frequency
*/
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

class MinHeapHuffmanTreeNodeComparator
{
    public:
        bool operator() (HuffmanTreeNode *&left, HuffmanTreeNode *&right) const
        {
            return left->frequency > right->frequency;
        };
};

/********************************/
/********** MAIN START **********/
/********************************/

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

/******************************/
/********** MAIN END **********/
/******************************/

/*************************************************/
/********** COMPRESSION ALGORITHM START **********/
/*************************************************/

/**
 * Compress the input file and save it to the output file
 * @param inputFile file descriptor for the file to be compressed into the output file
 * @param outputFile file descriptor for the output file to write the compressed file
*/
void compress(FILE *inputFile, FILE *outputFile)
{
    std::unordered_map<uint8_t, uint32_t> charactersFrequencyTable = countCharactersFrequency(inputFile);
    uint16_t distinctCharacterCount = charactersFrequencyTable.size();
    std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> nodeFrequencyQueue = createCharactersMinHeap(charactersFrequencyTable);
    HuffmanTreeNode * huffmanTreeRoot = buildHuffmanTree(nodeFrequencyQueue);
    uint32_t totalCharacterCount = huffmanTreeRoot->frequency;
    writeDistinctCharacterQuantityHeader(outputFile, distinctCharacterCount);
    writeTotalCharacterQuantityHeader(outputFile, totalCharacterCount);
    std::vector<uint8_t> preorderCharacters = getPreorderCharacters(huffmanTreeRoot);
    writeCharacters(outputFile, preorderCharacters);
    BitBuffer bitWritter(outputFile);
    writePreorderTreeHeader(huffmanTreeRoot, &bitWritter);
    std::unordered_map<uint8_t, class Node> characterTable = createCharacterBitTable(huffmanTreeRoot);
    compressInputFile(inputFile, &bitWritter, characterTable);
}

std::unordered_map<uint8_t, uint32_t> countCharactersFrequency(FILE *file)
{
    fpos_t previousFilePosition;
    fgetpos(file, &previousFilePosition);
    fseek(file, 0, SEEK_SET);
    std::unordered_map<uint8_t, uint32_t> charactersFrequency;
    while (true)
    {
        uint8_t byte;
        fread(&byte, sizeof(uint8_t), 1, file);
        if (feof(file) != 0)
        {
            break;
        }
        charactersFrequency[byte]++;
    }
    fsetpos(file, &previousFilePosition);
    return charactersFrequency;
}

/**
 * Creates a min-heap containing the characters and using its frequency as key
 * 
 * @return a min-heap priority queue of huffman tree nodes
*/
std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> createCharactersMinHeap(std::unordered_map<uint8_t, uint32_t> charactersFrequencyTable)
{
    std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> nodeFrequencyQueue;

    for (auto it = charactersFrequencyTable.begin(); it != charactersFrequencyTable.end(); ++it)
    {
        uint8_t byte = it->first;
        uint32_t frequency = it->second;
        HuffmanTreeNode * characterNode = new HuffmanTreeNode(byte, frequency);

        nodeFrequencyQueue.push(characterNode);
    }
    return nodeFrequencyQueue;
}

/**
 * Build the Huffman tree using a priority queue and return the root node
 * 
 * @param priorityQueue a min heap containing the leaf nodes
 * @return the Huffman tree root node
*/
class HuffmanTreeNode * buildHuffmanTree(std::priority_queue<class HuffmanTreeNode *, std::vector<class HuffmanTreeNode *>, class MinHeapHuffmanTreeNodeComparator> priorityQueue)
{
    while (priorityQueue.size() > 1)
    {

        HuffmanTreeNode *left = priorityQueue.top();
        priorityQueue.pop();

        HuffmanTreeNode *right = priorityQueue.top();
        priorityQueue.pop();
        
        HuffmanTreeNode *internalNode = new HuffmanTreeNode();
        internalNode->frequency = left->frequency + right->frequency;
        internalNode->left = left;
        internalNode->right = right;

        priorityQueue.push(internalNode);
    }
    HuffmanTreeNode *huffmanTree = priorityQueue.top();
    priorityQueue.pop();
    return huffmanTree;
}

/**
 * Writes the 2 bytes into the file
 * 
 * @param file the file to be written into
 * @param quantity a 2 byte number to be written
*/
void writeDistinctCharacterQuantityHeader(FILE *file, uint16_t quantity)
{
    fwrite(&quantity, sizeof(uint16_t), 1, file);
}

/**
 * Writes the 4 bytes into the file
 * 
 * @param file the file to be written into
 * @param quantity a 4 byte number to be written
*/
void writeTotalCharacterQuantityHeader(FILE *file, uint32_t quantity)
{
    fwrite(&quantity, sizeof(uint32_t), 1, file);
}

/**
 * Returns a new vector containing the characters from Huffman tree in pre order
 * 
 * @param root the root node of the Huffman tree
 * @return a vector containing the character in pre order
*/
std::vector<uint8_t> getPreorderCharacters(class HuffmanTreeNode *root)
{
    std::vector<uint8_t> characters;
    _getPreorderCharacters(root, &characters);
    return characters;
}

/**
 * This is an internal function, use getPreorderCharacters() instead.
*/
void _getPreorderCharacters(class HuffmanTreeNode *tree, std::vector<uint8_t> *vector)
{
    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    if (isLeaf)
    {
        vector->push_back(tree->byte);
        return;
    }
    _getPreorderCharacters(tree->left, vector);
    _getPreorderCharacters(tree->right, vector);
}

/**
 * Writes the characters from a vector into the file
 * 
 * @param file the file to be written into
 * @param characters the character vector of characters to be written
*/
void writeCharacters(FILE *file, std::vector<uint8_t> characters)
{
    for (auto it = characters.begin(); it != characters.end(); ++it)
    {
        fwrite(&*it, sizeof(uint8_t), 1, file);
    }
}

/**
 * Writes the tree structure in pre order into the file
 * 
 * @param tree the Huffman tree root node
 * @param bitBuffer the bit buffer writter of the file
*/
void writePreorderTreeHeader(class HuffmanTreeNode *tree, class BitBuffer *bitBuffer)
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
        bit = 1;
    }
    else
    {
        bit = 0;
    }
    bitBuffer->put_bit(bit);

    writePreorderTreeHeader(tree->left, bitBuffer);
    writePreorderTreeHeader(tree->right, bitBuffer);
}

/**
 * Creates a table mapping character and a Node that have the bits and their quantity
 * 
 * @param root the Huffman tree root node to be mapped
 * @return a map between a key character and its respective bits and bits quantity
*/
std::unordered_map<uint8_t, class Node> createCharacterBitTable(class HuffmanTreeNode *root)
{
    std::unordered_map<uint8_t, class Node> table;
    _createCharacterBitTable(root, &table, 0, 0);
    return table;
}

/**
 * This is an internal function, use createCharacterBitTable() instead.
*/
void _createCharacterBitTable(class HuffmanTreeNode *tree, std::unordered_map<uint8_t, class Node> *table, int quantity, uint8_t bits)
{
    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    if (isLeaf)
    {
        Node node(quantity, bits);
        (*table)[tree->byte] = node;
        return;
    }
    bits <<= 1;
    _createCharacterBitTable(tree->left, table, quantity + 1, bits | 0);
    _createCharacterBitTable(tree->right, table, quantity + 1, bits | 1);
}

/**
 * Reads the bytes of the input file and writes their respective bits into de output file
 * 
 * @param file the input file to be read
 * @param bitWritter the bit writter
 * @param characterTable the table mapping the character and the respective bits to be written
*/
void compressInputFile(FILE *file, BitBuffer *bitWritter, std::unordered_map<uint8_t, Node> characterTable)
{
    while (true)
    {
        uint8_t byte;
        fread(&byte, 1, 1, file);
        if (feof(file) != 0)
        {
            break;
        }
        Node node = characterTable[byte];
        for (int i = 0; i < node.bitQuantity; i++)
        {
            int bitShift = node.bitQuantity - i - 1;
            uint8_t bit = node.bits >> bitShift;
            bitWritter->put_bit(bit);
        }
    }
    bitWritter->write_byte();
}

/***********************************************/
/********** COMPRESSION ALGORITHM END **********/
/***********************************************/


/***************************************************/
/********** DECOMPRESSION ALGORITHM START **********/
/***************************************************/

/**
 * @param inputFile file descriptor for the compressed file to be decompressed
 * @param outputFile file descriptor for the file to be written the decompressed file
*/
void decompress(FILE *inputFile, FILE *outputFile)
{
    uint16_t distinctCharacters = readDistinctCharactersHeader(inputFile);
    uint32_t totalCharacters = readTotalCharactersHeader(inputFile);
    std::vector<uint8_t> characters = readCharactersHeader(inputFile, distinctCharacters);
    BitBuffer bitReader(inputFile);
    HuffmanTreeNode *huffmanTree = readTreeHeaderAndRebuildHuffmanTree(characters, &bitReader);
    std::unordered_map<class Node, uint8_t> characterTable = createCharacterTableForDecompression(huffmanTree);
    translateAndWriteCharacters(outputFile, &bitReader, characterTable, totalCharacters);
}

/**
 * Read the K distinct characters header
 * 
 * @param file file to be read the header
 * @return an unsigned 2 bytes of distinct quantity of characters
*/
uint16_t readDistinctCharactersHeader(FILE *file)
{
    uint16_t distinctCharacters;
    fread(&distinctCharacters, sizeof(uint16_t), 1, file);
    return distinctCharacters;
}

/**
 * Read the T total characters header
 * 
 * @param file file to be read the header
 * @return an unsigned 4 bytes of total quantity of characters
*/
uint32_t readTotalCharactersHeader(FILE *file)
{
    uint32_t totalCharacters;
    fread(&totalCharacters, sizeof(uint32_t), 1, file);
    return totalCharacters;
}

/**
 * Reads the pre order characters header
 * 
 * @param file file to be read the reader
 * @param characterCount how many characters must be read
 * @return a vector containg the characters read
*/
std::vector<uint8_t> readCharactersHeader(FILE *file, uint16_t characterCount)
{
    std::vector<uint8_t> characters;
    for (uint16_t i = 0; i < characterCount; i++)
    {
        uint8_t byte;
        fread(&byte, sizeof(uint8_t), 1, file);
        characters.push_back(byte);
    }
    return characters;
}

/**
 * Reads the pre order tree bits from the header and rebuild the Huffman tree
 * 
 * @param characters a vector of the header characters in pre order
 * @param bitBuffer the bit reader of input file
 * @return the root node of the Huffman tree
*/
class HuffmanTreeNode *readTreeHeaderAndRebuildHuffmanTree(std::vector<uint8_t> characters, class BitBuffer *bitBuffer)
{
    int index = 0;
    return _readTreeHeaderAndRebuildHuffmanTree(&characters, bitBuffer, &index);
}

/**
 * This is an internal function, use readTreeHeaderAndRebuildHuffmanTree() instead.
*/
class HuffmanTreeNode *_readTreeHeaderAndRebuildHuffmanTree(std::vector<uint8_t> *characters, class BitBuffer *bitBuffer, int *characterIndex)
{
    uint8_t bit = bitBuffer->get_bit();
    bool isLeaf = bit == 1;
    if (isLeaf)
    {
        uint8_t character = (*characters)[*characterIndex];
        *characterIndex += 1;
        HuffmanTreeNode *leafNode = new HuffmanTreeNode(character, 0);
        return leafNode;
    }
    HuffmanTreeNode *internalNode = new HuffmanTreeNode();
    internalNode->left = _readTreeHeaderAndRebuildHuffmanTree(characters, bitBuffer, characterIndex);
    internalNode->right = _readTreeHeaderAndRebuildHuffmanTree(characters, bitBuffer, characterIndex);
    return internalNode;
}

/**
 * Creates the table mapping each set of bits and quantity with the respective character
 * 
 * @param root the Huffman tree root node
 * @return a map mapping bits and quantity of bits with their respective character
*/
std::unordered_map<class Node, uint8_t> createCharacterTableForDecompression(class HuffmanTreeNode *root)
{
    std::unordered_map<class Node, uint8_t> characterTable;
    _createCharacterTableForDecompression(root, &characterTable, 0, 0);
    return characterTable;
}

/**
 * This is an internal function, use createCharacterTableForDecompression() instead.
*/
void _createCharacterTableForDecompression(class HuffmanTreeNode *tree, std::unordered_map<class Node, uint8_t> *table, int quantity, uint8_t bits)
{
    bool isLeaf = tree->left == nullptr && tree->right == nullptr;
    if (isLeaf)
    {
        Node leaf(quantity, bits);
        (* table)[leaf] = tree->byte;
        return;
    }
    bits <<= 1;
    _createCharacterTableForDecompression(tree->left, table, quantity + 1, bits | 0);
    _createCharacterTableForDecompression(tree->right, table, quantity + 1, bits | 1);
}

/**
 * Translates the bits read into their respective character using the table
 * for mapping between them
 * 
 * @param outputFile the file to be written the decompressed file
 * @param bitReader the bit buffer reader of the compressed file
 * @param characterTable the mapping of bits, bits quantity and their character
 * @param totalCharacters the total amount of characters to be written into the decompressed file
*/
void translateAndWriteCharacters(FILE *outputFile, class BitBuffer *bitReader, std::unordered_map<class Node, uint8_t> characterTable, uint32_t totalCharacters)
{
    Node node;
    uint32_t characterWrote = 0;
    while (characterWrote < totalCharacters)
    {
        uint8_t bit = bitReader->get_bit();

        node.bits <<= 1;
        node.bits |= bit;
        node.bitQuantity++;

        std::unordered_map<class Node, uint8_t>::iterator it = characterTable.find(node);

        if (it != characterTable.end())
        {
            uint8_t byte = characterTable[node];
            fwrite(&byte, sizeof(uint8_t), 1, outputFile);
            characterWrote++;
            node.bits = 0;
            node.bitQuantity = 0;
        }
    }
}
/*************************************************/
/********** DECOMPRESSION ALGORITHM END **********/
/*************************************************/
