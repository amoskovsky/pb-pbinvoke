set hhc=C:\Usr\App2\MsHtmlHelp\hhc.exe
perl gen_help.pl
pushd genhelp
%HHC% help.hhp
popd