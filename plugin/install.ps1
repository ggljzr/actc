# Just a simple script for copying plugin source code into
# Assetto Corsa plugin folder

$pluginName = "actc"
$appsFolder = "C:\Program Files (x86)\Steam\steamapps\common\assettocorsa\apps\python"

$pluginFolder = Join-Path -Path $appsFolder -Child $pluginName

if((Test-Path -Path $pluginFolder))
{
    Write-Output "Removing old files..."
    Remove-Item -Path $pluginFolder -Recurse
}

Write-Output "Creating plugin folder: $($pluginFolder)"
New-Item -Path $appsFolder -Name $pluginName -ItemType "directory"

Write-Output "Copying source files to $($pluginFolder)"
Copy-Item -Path "./src/*" -Destination $pluginFolder -Recurse