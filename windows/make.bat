@echo off

set INCLUDE_ORIGINAL=%INCLUDE%
set INCLUDE=%INCLUDE%;%ProgramFiles%\boost_1_70_0
set LIB_ORIGINAL=%LIB%
set LIB=%LIB%;%ProgramFiles%\boost_1_70_0\stage\lib
if [%1] == [main] (
    call:Main.exe
) else if [%1] == [clean] (
    call:clean
) else (
    call:error
)
set INCLUDE=%INCLUDE_ORIGINAL%
set INCLUDE_ORIGINAL=
set LIB=%LIB_ORIGINAL%
set LIB_ORIGINAL=
goto done


::Executables
:Main.exe
call:bin
if not exist bin\Main.exe (
	call:obj\driver.obj
	call:obj\HttpRequestMessage.obj
	link /out:bin\Main.exe obj\driver.obj obj\HttpRequestMessage.obj
)
goto:eof


::Object files
:obj\driver.obj
call:obj
if not exist obj\driver.obj (
    cl /c /EHsc /I%cd%\src\Headers /Foobj\driver.obj src\Sources\driver.cpp
)
goto:eof
:obj\HttpRequestMessage.obj
call:obj
if not exist obj\HttpRequestMessage.obj (
    cl /c /EHsc /I%cd%\src\Headers /Foobj\HttpRequestMessage.obj src\Sources\HttpRequestMessage.cpp
)
goto:eof


::Directories
:bin
if not exist bin (
    md bin
)
goto:eof
:obj
if not exist obj (
    md obj
)
goto:eof


::Utilities
:clean
set del_list=bin obj
for %%i in (%del_list%) do (
    if exist %%i (
	    del /q %%i\*
	)
)
set del_list=
goto:eof
:error
echo Invalid arguments
goto:eof


:done