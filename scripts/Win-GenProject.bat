@echo off
pushd ..\
call vendor\bin\premake\premake5.exe vs2015
popd
IF %ERRORLEVEL% NEQ 0(
	PAUSE
)