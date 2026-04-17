#Set-ExecutionPolicy -Scope CurrentUser RemoteSigned - trzeba czasami dodac przed odpaleniem
# Skrypt do testow na Windowsa
# .\run_all.ps1

Set-Location $PSScriptRoot
function Run-Test {
    param ($Name, $Exe, $Sources)

    Write-Host ""
    Write-Host "Running $Name..."

    g++ -Wall -o $Exe $Sources.Split(" ")

    if (Test-Path $Exe) {
        & .\$Exe
        Remove-Item $Exe
    }

    Write-Host ""
}

Run-Test "test_det" "test_det.exe" "test_det.cpp ../det.cpp ../../Point.cpp"

Run-Test "test_do_segments_intersect" "test_do_segments_intersect.exe" `
"test_do_segments_intersect.cpp ../../Point.cpp ../det.cpp ../is_point_on_segment.cpp"

Run-Test "test_graham_scan" "test_graham_scan.exe" `
"test_graham_scan.cpp ../graham_scan.cpp ../../Point.cpp ../det.cpp ../distance.cpp"

Run-Test "test_is_point_in_polygon" "test_is_point_in_polygon.exe" `
"test_is_point_in_polygon.cpp ../is_point_in_polygon.cpp ../../Point.cpp ../is_point_on_segment.cpp ../det.cpp ../do_segments_intersect.cpp"

Run-Test "test_is_point_on_segment" "test_is_point_on_segment.exe" `
"test_is_point_on_segment.cpp ../../Point.cpp ../det.cpp"

Run-Test "test_point" "test_point.exe" `
"test_point.cpp ../../Point.cpp"

Run-Test "test_class_BorderPatrol" "test_class_BorderPatrol.exe" `
"test_class_BorderPatrol.cpp ../../BorderPatrolSolver.cpp ../../Funkcje/graham_scan.cpp ../../Funkcje/det.cpp ../../Funkcje/distance.cpp ../../Point.cpp"