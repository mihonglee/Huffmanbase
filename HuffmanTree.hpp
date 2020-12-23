#ifndef HUFFTREEHPP
#define HUFFTREEHPP

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <stack>
#include <map>

class HuffmanTree : public HuffmanTreeBase {
public:
    HuffmanTree() { root_node = new HuffmanNode('\0', 0); }
    ~HuffmanTree();

    virtual std::string compress(const std::string inputStr);
    virtual std::string serializeTree() const;
    virtual std::string decompress(const std::string inputCode, const std::string serializedTree);
private:
    HuffmanNode* root_node;
};

void preorderCoding(std::map<char, std::string>* charcode, HuffmanNode* node, std::string code);
const std::string postorderSerialize(HuffmanNode* node);
void deleteNodes(HuffmanNode* node);

#endif /* HUFFTREEHPP */ 