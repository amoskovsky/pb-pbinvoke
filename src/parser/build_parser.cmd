@pushd %~dp0
@set path=C:\Usr\App\GnuWin32\bin;%path%
@set name=CParser
@set ext=y
@set BISON_PKGDATADIR=C:\Usr\App\GnuWin32\share\bison
@if "%1"=="1" del /q %name%.cpp

@perl -e "exit(((-M '%name%.%ext%' < -M '%name%.cpp') or !-f '%name%.cpp')  ? 1 : 0)"
@echo need rebuild=%errorlevel%
@if not errorlevel 1 goto _skip
@echo compile Y ...
if exist %name%.cpp del /q %name%.cpp
bison.exe --report=all --no-lines CParser.y  
if errorlevel 1 exit %errorlevel%
move /y CParser.tab.c  CParser.cpp
if errorlevel 1 exit %errorlevel%

:_skip
@popd
