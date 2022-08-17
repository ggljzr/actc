# Just a simple script for copying plugin source code into
# Assetto Corsa plugin folder

$pluginName = "actc"
$appsFolder = "C:\Program Files (x86)\Steam\steamapps\common\assettocorsa\apps\python"

$configName = "config.ini"

$pluginFolder = Join-Path -Path $appsFolder -Child $pluginName
$configFile = Join-Path -Path $pluginFolder -Child $configName
$configTempFile = Join-Path -Path $env:temp -Child $configName

if((Test-Path -Path $pluginFolder))
{
    # make a backup of old config file if it exists
    if((Test-Path -Path $configFile)) {
        Write-Output "Backing up config in $($configTempFile)"
        Copy-Item -Path $configFile -Destination $configTempFile
    }

    Write-Output "Removing old files..."
    Remove-Item -Path $pluginFolder -Recurse -Force
}

Write-Output "Creating plugin folder: $($pluginFolder)"
$null = New-Item -Path $appsFolder -Name $pluginName -ItemType "directory"

Write-Output "Copying source files to $($pluginFolder)"
Copy-Item -Path "./src/*" -Destination $pluginFolder -Recurse
# Copy config.ini

if((Test-Path -Path $configTempFile)) {
    Write-Output "Restoring config file..."
    Move-Item -Path $configTempFile -Destination $pluginFolder
}
else {
    Write-Output "No config backup found, creating new one..."
    Copy-Item -Path $configName -Destination $pluginFolder
}