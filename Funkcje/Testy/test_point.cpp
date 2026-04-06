#include<iostream>
#include<sstream>
#include "../point.h"

bool test1(){
    Point A(1,2);
    Point B(4,5);
    
    const bool expected = false;

    return ((A > B) == expected);
}

bool test2(){
    Point A(1,2);
    Point B(4,5);
    
    const bool expected = true;

    return ((A < B) == expected);
}

bool test3(){
    Point A(1, 2);

    try{
        std::ostringstream str;
        str<<A;
        return true;
    } catch(...){
        return false;
    }
}

int main(){
    std::cout<<"Testy dla klasy Point:"<<std::endl;
    std::cout<<"Test 1: "<<test1()<<std::endl;
    std::cout<<"Test 2: "<<test2()<<std::endl;
    std::cout<<"Test 3: "<<test3()<<std::endl;

    return 0;
}