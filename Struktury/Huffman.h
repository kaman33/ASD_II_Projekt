#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<memory>
#include<queue>
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>

struct HuffmanNode{
    char c;
    int freq;
    bool isLeaf;
    
    std::unique_ptr<HuffmanNode> left;
    std::unique_ptr<HuffmanNode> right;

    HuffmanNode() : c(0), freq(0), isLeaf(false), left(nullptr), right(nullptr) {}
    HuffmanNode(char character, int frequency) : c(character), freq(frequency), isLeaf(true), left(nullptr), right(nullptr) {}
};

struct HuffmanCode{
    char c;
    std::string code;

    HuffmanCode(char character, const std::string& huffmanCode) : c(character), code(huffmanCode) {}
};

std::unique_ptr<HuffmanNode> buildHuffmanTree(const std::vector<char>& C, const std::vector<int>& F);
std::vector<HuffmanCode> generateHuffmanCodes(const std::unique_ptr<HuffmanNode>& node, const std::string& prefix = "");
std::vector<HuffmanCode> Huffman(const std::vector<char>& C, const std::vector<int>& F);
void HuffmanPrint(const std::vector<HuffmanCode>& codes);
bool HuffmanEquals(const std::vector<HuffmanCode>& a, const std::vector<HuffmanCode>& b);
std::vector<HuffmanCode> HuffmanSort(const std::vector<HuffmanCode>& codes);

#endif