#include "../is_point_in_polygon.h"
#include<iostream>

bool test1(){
    std::vector<Point> polygon = {Point(0, 0), Point(5, 1), Point(3, 3), Point(5, 5), Point(2, 4), Point(0, 5)};
    Point p = Point(3, 2);

    const bool expected = true;

    return (is_point_in_polygon(p, polygon) == expected);
}

bool test2(){
    std::vector<Point> polygon = {Point(0, 0), Point(5, 1), Point(3, 3), Point(5, 5), Point(2, 4), Point(0, 5)};
    Point p = Point(4, 3);

    const bool expected = false;

    return (is_point_in_polygon(p, polygon) == expected);
}

int main(){
    std::cout<<"Testy dla funkcji is_point_in_polygon:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (in polygon)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (not in polygon)"<<std::endl;
}