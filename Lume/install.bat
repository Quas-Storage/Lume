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
        curl -L https://github.com/denoland/deno/releases/latest/download/deno-x86_64-pc-windows-msvc.zip -o deno.zip
        :: Extract the installer
        powershell -Command "Expand-Archive -Path deno.zip -DestinationPath %USERPROFILE%\deno"
        del deno.zip

        :: Add the Deno folder to PATH
        setx PATH "%USERPROFILE%\deno;%PATH%"
        echo Deno has been installed manually.
    )
) else (
    echo Deno is already installed.
)

deno install -A -f --global lume.ts

pause
