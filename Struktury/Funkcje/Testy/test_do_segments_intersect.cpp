#include <iostream>
#include "../do_segments_intersect.cpp"

// Odcinki rozlączne - nie przecinaja sie
bool test1(){
    Point A(1, 1), B(4, 2), C(2, 4), D(5, 5);
    const bool expected = false;

    return (do_segments_intersect(A, B, C, D) == expected);
}

// Odcinki przecinaja sie klasycznie X
bool test2()
{
    Point A(0,0), B(4,4), C(0,4), D(4,0);
    return(do_segments_intersect(A,B,C,D) == true);

}

// Odcinki stykaja sie w jednym koncowym punkcie
bool test3()
{
    Point A(0,0), B(2,2), C(2,2), D(4,0);
    return(do_segments_intersect(A,B,C,D) == true);
}

// Odcinki wspolliniowe i nachodzace na siebie
bool test4()
{
    Point A(0,0), B(4,0), C(2,0), D(6,0);
    return(do_segments_intersect(A,B,C,D) == true);
}

// Odcinki wspollionowe ale rozlaczne
bool test5()
{
    Point A(0,0), B(2,0), C(3,0), D(5,0);
    return(do_segments_intersect(A,B,C,D) == false);
}

int main(){
    std::cout<<"Testy dla funkcji do_segments_intersect:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (do not intersect)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<"(intersect)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<"(touch at endpoint)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<"(collinear overlapping)"<<std::endl; // wspollionowe nachodzace
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<"(collinear disjoing)"<<std::endl; // wspolliniowe rozlaczne

    return 0;
}