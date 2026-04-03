#include <iostream>
#include "do_segments_intersect.cpp"
using namespace std;

int main()
{
    // Test przeciazonych operatorow
    Punkt A(1,2);
    Punkt B(4,5);
    cout << (A > B) << endl; // 0
    cout << (A < B) << endl; // 1

    // Test fun is_point_on_segment - przykłady z wykładu
    Punkt p1(0,0), p2(4,2), p3(2,1);
    Punkt p4(6,3);
    cout << is_point_on_segment(p1,p2,p3) << endl; // 1
    cout << is_point_on_segment(p1,p2,p4) << endl; // 0

    // Test fun do_segments_intersect - przyklad z wykladu
    Punkt a(1,1), b(4,2), c(2,4), d(5,5);
    cout << do_segments_intersect(a,b,c,d); // 0;

    return 0;
}