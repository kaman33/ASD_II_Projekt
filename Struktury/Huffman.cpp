#include "Huffman.h"

std::unique_ptr<HuffmanNode> buildHuffmanTree(const std::vector<char>& C, const std::vector<int>& F){
    auto cmp = [](const std::unique_ptr<HuffmanNode>& a, const std::unique_ptr<HuffmanNode>& b){
        return a->freq > b->freq;
    };

    if((C.size() != F.size()) || C.empty() || F.empty()){
        return nullptr;
    }
    
    std::priority_queue<std::unique_ptr<HuffmanNode>, std::vector<std::unique_ptr<HuffmanNode>>, decltype(cmp)> pq(cmp);
    
    for(size_t i = 0; i < C.size(); i++){
        pq.push(std::move(std::make_unique<HuffmanNode>(C[i], F[i])));
    }
    
    while(pq.size() > 1){
        auto left = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
        pq.pop();
        auto right = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
        pq.pop();
        
        auto parent = std::make_unique<HuffmanNode>();
        parent->freq = left->freq + right->freq;
        parent->isLeaf = false;
        parent->left = std::move(left);
        parent->right = std::move(right);
        
        pq.push(std::move(parent));
    }
    
    if(pq.empty()) return nullptr;
    auto root = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
    pq.pop();

    return root;
}

std::vector<HuffmanCode> generateHuffmanCodes(const std::unique_ptr<HuffmanNode>& node, const std::string& prefix){
    std::vector<HuffmanCode> codes;

    if(node->isLeaf){
        codes.emplace_back(node->c, prefix);
    } else{
        if(node->left) {
            auto leftCodes = generateHuffmanCodes(node->left, prefix + "0");
            codes.insert(codes.end(), leftCodes.begin(), leftCodes.end());
        }
        if(node->right) {
            auto rightCodes = generateHuffmanCodes(node->right, prefix + "1");
            codes.insert(codes.end(), rightCodes.begin(), rightCodes.end());
        }
    }

    return codes;
}

std::vector<HuffmanCode> Huffman(const std::vector<char>& C, const std::vector<int>& F){
    auto root = buildHuffmanTree(C, F);

    if(root == nullptr){
        return {};
    }

    std::vector<HuffmanCode> huffmanCodes;
    huffmanCodes = generateHuffmanCodes(root);

    return huffmanCodes;
}

void HuffmanPrint(const std::vector<HuffmanCode>& codes){
    for(const auto& code : codes){
        std::cout<<code.c<<": "<<code.code<<std::endl;
    }
}

bool HuffmanEquals(const std::vector<HuffmanCode>& a, const std::vector<HuffmanCode>& b){
    if(a.size() != b.size()) return false;

    for(size_t i = 0; i < a.size(); i++){
        if((a[i].c != b[i].c) || (a[i].code != b[i].code)){
            return false;
        }
    }

    return true;
}

std::vector<HuffmanCode> HuffmanSort(const std::vector<HuffmanCode>& codes){
    if(codes.empty()) return {};

    std::vector<HuffmanCode> sortedCodes = codes;
    std::sort(sortedCodes.begin(), sortedCodes.end(), [](const HuffmanCode& a, const HuffmanCode& b){
        return a.c < b.c;
    });
    return sortedCodes;
}