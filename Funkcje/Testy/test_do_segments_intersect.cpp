#include <iostream>
#include "../do_segments_intersect.cpp"
using namespace std;

// g++ -Wall -o test_do_segments_intersect test_do_segments_intersect.cpp ../point.cpp ../det.cpp ../is_point_on_segment.cpp

// jak dopisalem jeszcze ../do_segments_intersect.cpp to byl blad, bez tego dziala

int main(){
    // Test fun do_segments_intersect - przyklad z wykladu
    Point a(1,1), b(4,2), c(2,4), d(5,5);
    cout << do_segments_intersect(a,b,c,d)<<endl; // 0;

    return 0;
}