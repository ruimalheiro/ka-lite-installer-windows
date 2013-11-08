@echo off
cd ..\ka-lite\kalite
for /f "tokens=1-2 delims==," %%I in (version.py) do ( if "%%I" == "VERSION " ( set version=%%J) )
set version=%version:"=%
set version=%version: =%
cd ..\..\installer-source
echo %version% > version.dat
exit
