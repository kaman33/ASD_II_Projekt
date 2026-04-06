#include <iostream>
#include "../is_point_on_segment.cpp"

bool test1(){
    Point p1(0,0), p2(4,2), p3(2,1);

    const bool expected = true;

    return (is_point_on_segment(p1,p2,p3) == expected);
}

bool test2(){
    Point p1(0,0), p2(4,2), p4(6,3);

    const bool expected = false;

    return (is_point_on_segment(p1,p2,p4) == expected);
}

int main(){
    std::cout<<"Testy dla funkcji is_point_on_segment:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<std::endl;

    return 0;
}