#include "../KMP.h"
#include<iostream>

bool test1(){
    std::string T = "ABCDBABCABBCDABCAB";
    std::string P = "ABCAB";
    
    const std::vector<int> expected = {5, 13};
    const std::vector<int> result = KMP(T, P);

    if(result.size() != expected.size()){
        return false;
    }

    for(unsigned int i{}; i<result.size(); i++){
        if(result[i] != expected[i]){
            return false;
        }
    }
    return true;
}

int main(){
    std::cout<<"Testy dla funkcji KMP:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (2 matches)"<<std::endl;

    return 0;
}