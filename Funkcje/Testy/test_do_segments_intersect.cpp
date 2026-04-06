#include <iostream>
#include "../do_segments_intersect.cpp"

bool test1(){
    Point A(1, 1), B(4, 2), C(2, 4), D(5, 5);

    const bool expected = false;

    return (do_segments_intersect(A, B, C, D) == expected);
}

int main(){
    std::cout<<"Testy dla funkcji do_segments_intersect:"<<std::endl;
    std::cout<<"Test 1: "<<test1()<<std::endl;

    return 0;
}