#!/bin/bash

./run_test_point.sh
echo ""
./run_test_det.sh
echo ""
./run_test_is_point_on_segment.sh
echo ""
./run_test_do_segments_intersect.sh
echo ""
./run_test_is_point_in_polygon.sh
echo ""
./run_test_graham_scan.sh
echo ""
./run_test_class_BorderPatrol.sh
echo ""
./run_test_huffman.sh
echo ""
./run_test_min_cost_max_flow.sh
echo ""
./run_test_kmp.sh
