#include "../det.h"
#include<iostream>

bool test1(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(1, 3);

    const float expected = 8;

    return (det(p1, p2, p3) == expected);
}

bool test2(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(4, -1);

    const float expected = -7;

    return (det(p1, p2, p3) == expected);
}

bool test3(){
    Point p1(0, 0);
    Point p2(3, 1);
    Point p3(6, 2);

    const float expected = 0;

    return (det(p1, p2, p3) == expected);
}

bool test4(){
    Point p1(0 ,0);
    Point p2(4, 2);
    Point p3(2, 1);

    const float expected = 0;

    return (det(p1, p2, p3) == expected);
}

bool test5(){
    Point p1(0, 0);
    Point p2(4, 2);
    Point p3(6, 3);

    const float expected = 0;

    return (det(p1, p2, p3) == expected);
}

bool test6(){
    Point p1(0, 0);
    Point p2(4, 2);
    Point p3(3, 3);

    const float expected = 6;

    return (det(p1, p2, p3) == expected);
}

int main(){
    std::cout<<"Testy dla funkcji det:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (det = 8)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (det = -7)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<" (det = 0)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<" (det = 0)"<<std::endl;
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<" (det = 0)"<<std::endl;
    std::cout<<"Test 6: "<<(test6()?"OK":"ERROR")<<" (det = 6)"<<std::endl;
}