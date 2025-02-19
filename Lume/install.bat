@echo off

echo LUME PROGRAMMING LANGUAGE CLI

pause

:: Check if Deno is installed
deno --version >nul 2>&1

:: If Deno is not installed, install it
if %errorlevel% neq 0 (
    echo Deno is not installed. Installing Deno...

    :: Check if winget is available and install Deno using it
    where winget >nul 2>&1
    if %errorlevel% == 0 (
        winget install --id denoland.deno
        echo Deno has been installed using winget.
    ) else (
        echo winget is not available. Attempting to install manually...
        :: Download the Deno installer
        powershell -Command "irm https://deno.land/install.ps1 | iex"

        echo Deno has been installed manually.
    )
) else (
    echo Deno is already installed.
)

:: Get the full path of the current script
set SCRIPT_DIR=%~dp0

:: Use the full path to run the lume.ts installation
deno install -A -f --global "%SCRIPT_DIR%lume.ts"

pause
