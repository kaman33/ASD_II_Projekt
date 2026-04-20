Set-Location $PSScriptRoot

$Exe = "test_data_loader.exe"
$Sources = @(
    "test_data_loader.cpp",
    "..\DataLoader.cpp",
    "..\..\Struktury\Krasnal.cpp",
    "..\..\Struktury\Kopalnia.cpp",
    "..\..\Struktury\Straznik.cpp",
    "..\..\Struktury\Point.cpp"
)

g++ -Wall -std=c++14 -o $Exe @Sources
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

& ".\$Exe"
$TestExitCode = $LASTEXITCODE

if (Test-Path $Exe) {
    Remove-Item $Exe
}

exit $TestExitCode
