@echo off

rd /q /s sdk_components 2>nul
mklink /j sdk_components external\sdk13\components
rd /q /s external\nano-pb 2>nul
mklink /j external\nano-pb external\sdk13\external\nano-pb
rd /q /s external\segger_rtt 2>nul
mklink /j external\segger_rtt external\sdk13\external\segger_rtt


WHERE >nul 2>nul git 
IF %ERRORLEVEL% NEQ 0 (
    ECHO "git was not found in PATH, please install and append to your PATH"
    goto return
)

WHERE >nul 2>nul make 
IF %ERRORLEVEL% NEQ 0 (
    ECHO "make was not found in PATH, please install and append to your PATH"
    goto return
)

IF NOT EXIST external\sdk13\external\micro-ecc\micro-ecc\uECC.c (
    ECHO "micro-ecc not found! Let's pull it from HEAD."
    git clone https://github.com/kmackay/micro-ecc.git external\sdk13\external\micro-ecc\micro-ecc
)
pushd external\sdk13\external\micro-ecc\nrf52_armgcc\armgcc && make && popd
pushd external\sdk13\external\micro-ecc\nrf52_keil\armgcc && make && popd

:return
if "%1%"=="nopause" goto done
pause
:done
