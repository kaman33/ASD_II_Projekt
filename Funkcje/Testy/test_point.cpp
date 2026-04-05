#include <iostream>
#include "../point.h"
using namespace std;

// g++ -Wall -o test_point test_point.cpp ../point.cpp

int main(){
    // Test przeciazonych operatorow
    Point A(1,2);
    Point B(4,5);
    cout << (A > B) << endl; // 0
    cout << (A < B) << endl; // 1

    return 0;
}