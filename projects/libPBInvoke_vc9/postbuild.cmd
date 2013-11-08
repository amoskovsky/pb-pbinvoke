rem echo on
rem echo %*
if "%OUTDIR%"=="" set OUTDIR=%1&& shift
if "%OUTDIR%"=="" echo OUTDIR was not specified && exit 1

set cfg=%1&& shift

rem if "%cfg%"=="ReleaseDemo" exit /b 0

set todir=..\..\pbsrc\tester

copy %outdir%\*.dll %todir%\*.* /y