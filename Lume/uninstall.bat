pause

deno uninstall --global lume.ts

@echo off
setlocal

:: Ask user for confirmation
echo Are you sure you want to uninstall Deno? (Y/N)
set /p userinput=Choice:

:: Convert user input to uppercase
set userinput=%userinput:~0,1%
set userinput=%userinput: =%
set userinput=%userinput:~-1%

if /I "%userinput%"=="Y" (
    echo Uninstalling Deno...

    :: Remove Deno using the command for your system
    if exist "%USERPROFILE%\AppData\Local\deno" (
        rd /s /q "%USERPROFILE%\AppData\Local\deno"
        rd /s /q "%USERPROFILE%\.deno"
        rd /s /q "%USERPROFILE%\deno"
    )
    if exist "%USERPROFILE%\AppData\Local\Programs\deno" (
        rd /s /q "%USERPROFILE%\AppData\Local\Programs\deno"
        rd /s /q "%USERPROFILE%\.deno"
        rd /s /q "%USERPROFILE%\deno"
    )

    echo Deno has been uninstalled.
) else (
    echo Deno uninstallation canceled.
)

endlocal
pause
