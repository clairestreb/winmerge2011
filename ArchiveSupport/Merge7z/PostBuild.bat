@echo off

if not exist %1 rem: goto :usage
if not exist %2 rem: goto :usage

REM Locate 7z.exe
set $=%ProgramFiles%\7-zip;%ProgramFiles(x86)%\7-zip
for %%$ in (7z.exe) do if not "%%~$$:$" == "" set SevenZip=%%~$$:$

cd %~2
"%SevenZip%" e -so %~1 _7z.dll > 7z.dll
"%SevenZip%" e -y %~1 History.txt License.txt
md Lang
cd Lang
"%SevenZip%" e -y %~1 *.ttt *.txt -x!History.txt -x!License.txt -x!readme.txt
for %%$ in (*_*.txt) do call :hyphenize %%$

exit

:hyphenize
set $=%1
del %$:_=-%
rename %$% %$:_=-%
goto :eof

:usage
echo ####################################################################
echo # Merge7z.vcproj post-build script                                 #
echo # Not intended for direct invocation through user interface        #
echo # Post-build command line: PostBuild.bat "<msi_file>" "$(OutDir)"  #
echo ####################################################################
pause
