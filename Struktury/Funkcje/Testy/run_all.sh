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
