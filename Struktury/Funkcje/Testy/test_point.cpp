#include<iostream>
#include<sstream>
#include "../../Point.h"

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

bool test4(){
    Point A(1, 2);
    Point B(1, 2);

    const bool expected = true;

    return ((A == B) == expected);
}

bool test5(){
    Point A(1, 2);
    Point B(3, 4);

    const bool expected = true;

    return ((A != B) == expected);
}

bool test6(){
    Point A(1, 2);
    Point B(1, 2);

    const bool expected = true;

    return ((A <= B) == expected);
}

bool test7(){
    Point A(1, 2);
    Point B(0, 0);

    const bool expected = true;

    return ((A >= B) == expected);
}

bool test8(){
    Point A(1, 2);
    Point B(3, 4);
    B = A;

    const bool expected = true;

    return ((A == B) == expected);
}

int main(){
    std::cout<<"Testy dla klasy Point:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (operator >)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (operator <)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<" (operator <<)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<" (operator ==)"<<std::endl;
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<" (operator !=)"<<std::endl;
    std::cout<<"Test 6: "<<(test6()?"OK":"ERROR")<<" (operator <=)"<<std::endl;
    std::cout<<"Test 7: "<<(test7()?"OK":"ERROR")<<" (operator >=)"<<std::endl;
    std::cout<<"Test 8: "<<(test8()?"OK":"ERROR")<<" (operator =)"<<std::endl;

    return 0;
}