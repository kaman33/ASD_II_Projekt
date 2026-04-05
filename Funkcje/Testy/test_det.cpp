#include "../det.h"
#include<iostream>

// g++ -Wall -o test_det test_det.cpp ../det.cpp ../point.cpp

int test1(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(1, 3);

    // 8
    return det(p1, p2, p3);
}

int test2(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(4, -1);

    // -7
    return det(p1, p2, p3);
}

int test3(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(6, 2);

    // 0
    return det(p1, p2, p3);
}

int test4(){
    Point p1(0 ,0);
    Point p2(4, 2);
    Point p3(2, 1);

    // 0
    return det(p1, p2, p3);
}

int test5(){
    Point p1(0, 0);
    Point p2(4, 2);
    Point p3(6, 3);

    // 0
    return det(p1, p2, p3);
}

int test6(){
    Point p1(0, 0);
    Point p2(4, 2);
    Point p3(3, 3);

    // 6
    return det(p1, p2, p3);
}

int main(){
    // wszedzie powinno byc true
    std::cout<<"Test 1: "<<(test1() == 8)<<std::endl;
    std::cout<<"Test 2: "<<(test2() == -7)<<std::endl;
    std::cout<<"Test 3: "<<(test3() == 0)<<std::endl;
    std::cout<<"Test 4: "<<(test4() == 0)<<std::endl;
    std::cout<<"Test 5: "<<(test5() == 0)<<std::endl;
    std::cout<<"Test 6: "<<(test6() == 6)<<std::endl;
}