@echo off

if '%MSVCDIR%' == '' goto lookfor
:found

if "%OS%" == "Windows_NT" set _SYS_=%SYSTEMROOT%\system32
if "%OS%" == "" set _SYS_=%windir%\system

copy *.dll %_SYS_%
xcopy /e /y include %MSVCDIR%\include
xcopy /e /y lib %MSVCDIR%\lib

echo.
echo Allegro has been installed
echo.

goto end

:lookfor
call vcvars32
if '%MSVCDIR%' == '' goto notfound
goto found

:notfound
echo.
echo The Environment variable MSVCDIR is not set, you will have to copy the files by
echo hand. See the README file for more details.
echo.



:end

pause