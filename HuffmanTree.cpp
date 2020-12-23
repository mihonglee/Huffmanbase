#include "HuffmanTree.hpp"

HuffmanTree::~HuffmanTree() {
    // call deleteNodes with the root nodes to delete all the nodes
    deleteNodes(root_node);
}

// function called recursively to delete the linked tree using postorder
void deleteNodes(HuffmanNode* node) {
    if (!node->isLeaf()) { //If it is not a leaf
        // delete the node's child
        deleteNodes(node->left);
        deleteNodes(node->right);
    }
    delete node; //actual deletion of the node
}

// compress function to generate the code
std::string HuffmanTree::compress(const std::string inputStr) {
    std::map<char, int> charfreq; // map with the char and that char's frequency
    for (unsigned int i = 0; i < inputStr.size(); i++) {
        if(charfreq.count(inputStr.at(i)) == 1) { // if the char exists as a key in the map
            charfreq.at(inputStr.at(i)) += 1; //increasing the frequency of that char in the map
        }
        else {
            auto p = std::make_pair(inputStr.at(i), 1);
            charfreq.insert(p); //inserted the new char in the map with freq of 1
        }
    }

    //Creating HeapQueue object
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> pq; //HeapQueue
    auto iter = charfreq.begin();
    while(iter != charfreq.end()) { //Using HeapQueue to build the binary tree
        HuffmanNode* new_node = new HuffmanNode(iter->first, iter->second);
        pq.insert(new_node);
        iter++;
    }

    const char nullchar = '\0';
    while(pq.size() != 1) { // Until there is one element left in the HeapQueue
        auto node1 = pq.min();
        pq.removeMin(); //remove once
        auto node2 = pq.min();
        pq.removeMin(); // remove twice
        auto intermediate_freq = (node1->getFrequency() + node2->getFrequency());
        HuffmanNode* intermediate_node = //temporary node
            new HuffmanNode(nullchar, intermediate_freq, nullptr, node1, node2);
        node1->parent = intermediate_node;
        node2->parent = intermediate_node;
        pq.insert(intermediate_node); //inserting the intermediate node back into the queue
    }

    // Only one pair left in HeapQueue for root data member
    root_node = pq.min();

    std::string strcode = "";
    std::map<char, std::string> charcode; //map with the char and the associated code
    //sending in the map, root node, and an empty string to put the correct code in the map
    preorderCoding(&charcode, root_node, strcode);

    // comparing each char of the input string with the chars in the map
    for(auto i : inputStr) {
        strcode += charcode.at(i); 
    }
    return strcode;
}

// function to find the code of the char recursively using preorder
void preorderCoding(std::map<char, std::string>* charcode, HuffmanNode* node, 
    std::string code) {
        if (!node->isLeaf()) { //if it is not a leaf
            // call each child
            preorderCoding(charcode, node->left, code + '0');
            preorderCoding(charcode, node->right, code + '1');
        }
        else {
            //visit the node
            auto p = std::make_pair(node->getCharacter(), code);
            charcode->insert(p); //insert the char, code pair
        }
        // will only reach return statement when finished
        return;
}

std::string HuffmanTree::serializeTree() const{ 
    //recursivley call postorderSerialize starting at the root
    return postorderSerialize(root_node);
}

const std::string postorderSerialize(HuffmanNode* node) {

    std::string serialize_str = "";
    if (node->isLeaf()) { //when it is a leaf
        //visit the node
        serialize_str += "L"; // add 'L' when leaf
        serialize_str += node->getCharacter();
        return serialize_str;
    }
    
    // add a 'B' if it is a branch for each child of the node
    return (postorderSerialize(node->left) + postorderSerialize(node->right) + 'B');
}

// decompress to retrieve the original string
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    std::stack<HuffmanNode*> charStack; //stack to hold the chars

    for(unsigned int i = 0; i < serializedTree.length(); i++) {
        if(serializedTree[i] == 'L') { //is leaf
            i++;
            char ch = serializedTree[i];
            HuffmanNode* new_node = new HuffmanNode(ch, 0);
            charStack.push(new_node);
        }
        else { //is branch
            HuffmanNode* right_child = charStack.top();
            charStack.pop(); //remove one
            HuffmanNode* left_child = charStack.top();
            charStack.pop(); //remove two

            //create parent node
            HuffmanNode* par_node = new HuffmanNode(0, 0, nullptr, left_child, right_child);

            //push parent node back to the stack
            charStack.push(par_node);
        }
    }
    HuffmanNode* root = charStack.top(); //last one is the root
    std::string decoded = ""; //return string

    unsigned int i = 0;
    HuffmanNode* curr_node;
    bool atRoot = false; //boolean var will be used in the loop
    while(i < inputCode.length()) {
        if(atRoot) {
            if(curr_node->isLeaf()) { //if curr_node is leaf
                decoded += (curr_node->getCharacter());
                atRoot = false;
            }
            else { //if curr_node is a branch
                if(inputCode[i] == '1') { 
                    // if a 1 it is a right child
                    curr_node = curr_node->right;
                }
                else {
                    // if a 0 it is a left child
                    curr_node = curr_node->left;
                }
                i++;
            }
        }
        else { //when the curr_node is the root
            curr_node = root;
            atRoot = true;
        }
    }

    // add the last char
    decoded += (curr_node->getCharacter());
    return decoded;
}
