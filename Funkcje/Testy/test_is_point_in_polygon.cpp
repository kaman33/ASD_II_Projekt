#include "../is_point_in_polygon.h"
#include<iostream>

// g++ -Wall -o test_is_point_in_polygon test_is_point_in_polygon.cpp ../is_point_in_polygon.cpp ../point.cpp ../is_point_on_segment.cpp ../det.cpp ../do_segments_intersect.cpp

// testy z przykladu z wykladu

bool test1(){
    std::vector<Point> polygon = {Point(0, 0), Point(5, 1), Point(3, 3), Point(5, 5), Point(2, 4), Point(0, 5)};
    Point p = Point(3, 2);

    return is_point_in_polygon(p, polygon);
}

bool test2(){
    std::vector<Point> polygon = {Point(0, 0), Point(5, 1), Point(3, 3), Point(5, 5), Point(2, 4), Point(0, 5)};
    Point p = Point(4, 3);

    return is_point_in_polygon(p, polygon);
}

int main(){
    std::cout<<"Test 1: "<<test1()<<std::endl;
    std::cout<<"Test 2: "<<test2()<<std::endl;
}