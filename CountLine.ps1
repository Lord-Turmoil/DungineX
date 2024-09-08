# save current directory
$pwd = Get-Location

cd DungineX
$totalC = (dir -include *.cpp,*.c -recurse | select-string .).Count
$totalH = (dir -include *.h -recurse | select-string .).Count

cd Vendor
$vendorC = (dir -include *.cpp,*.c -recurse | select-string .).Count
$vendorH = (dir -include *.h -recurse | select-string .).Count

$actualC = $totalC - $vendorC
$actualH = $totalH - $vendorH

Write-Host "Total C/C++ files: $actualC"
Write-Host "Total     H files: $actualH"

cd $pwd
