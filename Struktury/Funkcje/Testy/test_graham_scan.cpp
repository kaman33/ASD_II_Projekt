#include"../graham_scan.h"
#include<iostream>

bool test1(){
    std::vector<Point> points, expected;

    points = {};

    expected = {};
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

bool test2(){
    std::vector<Point> points, expected;

    points = {Point(0, 0)};

    expected = points;
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

bool test3(){
    std::vector<Point> points, expected;

    points = {Point(0, 0), Point(2, 0)};

    expected = points;
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

bool test4(){
    std::vector<Point> points, expected;

    points = {Point(0, 0), Point(2, 0), Point(1, 2)};

    expected = points;
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

bool test5(){
    std::vector<Point> points, expected;

    points = {Point(0, 0), Point(2, 0), Point(1, 2), Point(1, 1)};

    expected = {Point(0, 0), Point(2, 0), Point(1, 2)};
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

bool test6(){
    std::vector<Point> points, expected;

    points = {
        Point(0, 0), Point(1, 0), Point(2, 0), Point(2, 1), Point(4, 2),
        Point(3, 4), Point(1, 3), Point(5, 1), Point(3, 0), Point(2, 2)
    };

    expected = {Point(0, 0), Point(3, 0), Point(5, 1), Point(3, 4), Point(1, 3)};
    points = graham_scan(points);

    if(points.size() != expected.size()) return false;

    for(unsigned int i = 0; i < points.size(); i++){
        if(points[i] != expected[i]) return false;
    }

    return true;
}

int main(){
    std::cout<<"Testy dla funkcji graham_scan:"<<std::endl;
    std::cout<<"Test 1: "<<(test1()?"OK":"ERROR")<<" (empty set)"<<std::endl;
    std::cout<<"Test 2: "<<(test2()?"OK":"ERROR")<<" (1 element set)"<<std::endl;
    std::cout<<"Test 3: "<<(test3()?"OK":"ERROR")<<" (2 element set)"<<std::endl;
    std::cout<<"Test 4: "<<(test4()?"OK":"ERROR")<<" (3 element set)"<<std::endl;
    std::cout<<"Test 5: "<<(test5()?"OK":"ERROR")<<" (4 element set)"<<std::endl;
    std::cout<<"Test 6: "<<(test6()?"OK":"ERROR")<<" (10 element set)"<<std::endl;

    return 0;
}