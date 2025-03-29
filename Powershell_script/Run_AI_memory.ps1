# Run model script in WSL
# wsl -d Ubuntu-24.04 -e bash Linux_scripts/run_local_AI_model.sh

# Define the Windows path to the script
$winScriptPath = "C:\\Rohit\\Self_evolving_AI\\Run_Local_AI_Model\\Linux_scripts\\run_local_AI_model.sh"

# Convert Windows path to WSL path
$wslScriptPath =  wsl -d Ubuntu-24.04 wslpath "$winScriptPath"

Write-Host "Path: $wslScriptPath"

Start-Process "wsl.exe" -ArgumentList "-d", "Ubuntu-24.04", "--", "bash", "-c", "$wslScriptPath"
