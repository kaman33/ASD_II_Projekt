#include "../../Huffman.h"
#include<iostream>
using namespace std;

bool test1(){
    vector<char> characters = {'a', 'b', 'c', 'd', 'e', 'f'};
    vector<int> frequencies = {45, 13, 12, 16, 9, 5};

    vector<HuffmanCode> expectedCodes = {
        {'a', "0"},
        {'b', "101"},
        {'c', "100"},
        {'d', "111"},
        {'e', "1101"},
        {'f', "1100"}
    };

    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));
    
    return HuffmanEquals(huffmanCodes, expectedCodes);
}

bool test2(){
    vector<char> characters = {'a', 'b', 'c', 'd', 'e', 'f'};
    vector<int> frequencies = {1, 1, 1, 1, 1, 1};

    vector<HuffmanCode> expectedCodes = {
        {'a', "110"},
        {'b', "00"},
        {'c', "111"},
        {'d', "01"},
        {'e', "101"},
        {'f', "100"}
    };

    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));

    return HuffmanEquals(huffmanCodes, expectedCodes);
}

bool test3(){
    vector<char> characters = {'m', 'k', 'x', 'y', 'z'};
    vector<int> frequencies = {18, 9, 2, 3, 7};
    
    vector<HuffmanCode> expectedCodes = {
        {'k', "10"},
        {'m', "0"},
        {'x', "1100"},
        {'y', "1101"},
        {'z', "111"}
    };

    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));

    return HuffmanEquals(huffmanCodes, expectedCodes);
}

bool test4(){
    vector<char> characters = {};
    vector<int> frequencies = {};

    vector<HuffmanCode> expectedCodes = {};

    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));

    return HuffmanEquals(huffmanCodes, expectedCodes);
}

bool test5(){
    vector<char> characters = {'x', 'y', 'z'};
    vector<int> frequencies = {};

    vector<HuffmanCode> expectedCodes = {};

    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));

    return HuffmanEquals(huffmanCodes, expectedCodes);
}

bool test6(){
    vector<char> characters = {};
    vector<int> frequencies = {2, 3, 5, 7};

    vector<HuffmanCode> expectedCodes = {};
    
    vector<HuffmanCode> huffmanCodes = HuffmanSort(Huffman(characters, frequencies));

    return HuffmanEquals(huffmanCodes, expectedCodes);
}

int main(){
    std::cout<<"Testy dla funkcji huffman:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (normal set)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (same freq)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<" (normal set)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<" (both empty)"<<std::endl;
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<" (freq empty)"<<std::endl;
    std::cout<<"Test 6: "<<(test6()?"OK":"ERROR")<<" (chars empty)"<<std::endl;

    return 0;
}