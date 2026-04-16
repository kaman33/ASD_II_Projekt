#Set-ExecutionPolicy -Scope CurrentUser RemoteSigned - trzeba czasami dodac przed odpaleniem

# Skrypt do testow na Windowsa
# .\run_all.ps1
Write-Host "Running test_det..."
g++ -Wall -o test_det.exe test_det.cpp ../det.cpp ../../Point.cpp
if (Test-Path "test_det.exe") {
    ./test_det.exe
    Remove-Item test_det.exe
}
Write-Host ""

Write-Host "Running test_do_segments_intersect..."
g++ -Wall -o test_do_segments_intersect.exe test_do_segments_intersect.cpp ../../Point.cpp ../det.cpp ../is_point_on_segment.cpp
if (Test-Path "test_do_segments_intersect.exe") {
    ./test_do_segments_intersect.exe
    Remove-Item test_do_segments_intersect.exe
}
Write-Host ""

Write-Host "Running test_graham_scan..."
g++ -Wall -o test_graham_scan.exe test_graham_scan.cpp ../graham_scan.cpp ../../Point.cpp ../det.cpp ../distance.cpp
if (Test-Path "test_graham_scan.exe") {
    ./test_graham_scan.exe
    Remove-Item test_graham_scan.exe
}
Write-Host ""

Write-Host "Running test_is_point_in_polygon..."
g++ -Wall -o test_is_point_in_polygon.exe test_is_point_in_polygon.cpp ../is_point_in_polygon.cpp ../../Point.cpp ../is_point_on_segment.cpp ../det.cpp ../do_segments_intersect.cpp
if (Test-Path "test_is_point_in_polygon.exe") {
    ./test_is_point_in_polygon.exe
    Remove-Item test_is_point_in_polygon.exe
}
Write-Host ""

Write-Host "Running test_is_point_on_segment..."
g++ -Wall -o test_is_point_on_segment.exe test_is_point_on_segment.cpp ../../Point.cpp ../det.cpp
if (Test-Path "test_is_point_on_segment.exe") {
    ./test_is_point_on_segment.exe
    Remove-Item test_is_point_on_segment.exe
}
Write-Host ""

Write-Host "Running test_point..."
g++ -Wall -o test_point.exe test_point.cpp ../../Point.cpp
if (Test-Path "test_point.exe") {
    ./test_point.exe
    Remove-Item test_point.exe
}
Write-Host ""