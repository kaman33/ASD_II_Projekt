#include <iostream>
#include "../is_point_on_segment.cpp"
using namespace std;

// g++ -Wall -o test_is_point_on_segment test_is_point_on_segment.cpp ../point.cpp ../det.cpp

// jak dopisalem jeszcze ../is_point_on_segment.cpp to byl blad, bez tego dziala

int main(){
    // Test fun is_point_on_segment - przykłady z wykładu
    Point p1(0,0), p2(4,2), p3(2,1);
    Point p4(6,3);
    cout << is_point_on_segment(p1,p2,p3) << endl; // 1
    cout << is_point_on_segment(p1,p2,p4) << endl; // 0

    return 0;
}