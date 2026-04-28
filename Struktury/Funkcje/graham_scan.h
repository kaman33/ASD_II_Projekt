#ifndef GRAHAM_SCAN_H
#define GRAHAM_SCAN_H

#include<algorithm>
#include<iostream>
#include<vector>
#include"det.h"
#include"../Point.h"
#include "EPS.h"
#include "distance.h"

int nearestIdx(std::vector<Point> &points);
std::vector<Point> graham_scan(std::vector<Point> points);

#endif