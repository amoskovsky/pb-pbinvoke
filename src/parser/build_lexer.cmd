pushd %~dp0
set name=CLexer
set ext=l
perl -e "exit(((-M '%name%.%ext%' < -M '%name%.cpp') or !-f '%name%.cpp')  ? 1 : 0)"
@echo need rebuild=%errorlevel%
if not errorlevel 1 goto _skip
@echo compile L ...
C:\Usr\App\Git\bin\flex.exe %name%.%ext%
if errorlevel 1 exit %errorlevel%

:_skip
popd
