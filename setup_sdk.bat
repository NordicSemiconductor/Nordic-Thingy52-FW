@echo off

python setup_sdk.py

rmdir sdk_components
mklink /j sdk_components external\sdk\components
rmdir external\nano-pb
mklink /j external\nano-pb external\sdk\external\nano-pb
rmdir external\segger_rtt
mklink /j external\segger_rtt external\sdk\external\segger_rtt
rmdir external\micro-ecc
mklink /j external\micro-ecc external\sdk\external\micro-ecc\micro-ecc
if "%1%"=="nopause" goto done
pause
:done
