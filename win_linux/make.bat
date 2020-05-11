@echo off

set INCLUDE_ORIGINAL=%INCLUDE%
set INCLUDE=%INCLUDE%;%ProgramFiles%\boost\boost_1_72_0
if [%1] == [main] (
    call:Main.exe
) else if [%1] == [clean] (
    call:clean
) else (
    call:error
)
set INCLUDE=%INCLUDE_ORIGINAL%
set INCLUDE_ORIGINAL=
goto done


::EXECUTABLES
:Main.exe
set obj_dependency_list=obj\driver.obj
set lib_dependency_list=lib\Network.lib libcrypto.lib libssl.lib
call:bin
if not exist bin\Main.exe (
    call:Main_exe_obj
    call:Main_exe_lib
    link /machine:x64 /libpath:"%cd%\resources\win\boost_1_72_0\lib\x64\static" /libpath:"%cd%\resources\win\openssl\lib" /out:bin\Main.exe %obj_dependency_list% %lib_dependency_list%
    copy "%cd%\resources\win\openssl\lib\*.dll" .\bin
)
set obj_dependency_list=
set lib_dependency_list=
goto:eof
:Main_exe_obj
set src_dependency_list=src\Sources\driver.cpp
call:obj
for %%i in (%src_dependency_list%) do (
    if not exist obj\%%~ni.obj (
        cl /c /EHsc /Od /I"%cd%\resources\win\openssl\include" /I"%cd%\src\Headers" /D_WIN32_WINNT=0x0601 /Foobj\%%~ni.obj %%i
    )
)
set src_dependency_list=
goto:eof
:Main_exe_lib
set lib_dependency_list=lib\Network.lib
for %%i in (%lib_dependency_list%) do (
    call:%%i
)
set lib_dependency_list=
goto:eof




:lib\Network.lib
set obj_dependency_list=obj\HttpByteContent.obj^
 obj\HttpContent.obj^
 obj\HttpFileStreamContent.obj^
 obj\HttpRequestMessage.obj^
 obj\HttpResponseMessage.obj^
 obj\HttpStringContent.obj^
 obj\WebClient.obj
call:lib
if not exist lib\Network.lib (
    call:Network_lib_obj
    lib /out:lib\Network.lib %obj_dependency_list%
)
goto:eof
:Network_lib_obj
set src_dependency_list=src\Sources\HttpByteContent.cpp^
 src\Sources\HttpContent.cpp^
 src\Sources\HttpFileStreamContent.cpp^
 src\Sources\HttpRequestMessage.cpp^
 src\Sources\HttpResponseMessage.cpp^
 src\Sources\HttpStringContent.cpp^
 src\Sources\WebClient.cpp
call:obj
for %%i in (%src_dependency_list%) do (
    if not exist obj\%%~ni.obj (
        cl /c /EHsc /Od /I"%cd%\resources\win\openssl\include" /I"%cd%\src\Headers" /D_WIN32_WINNT=0x0601 /Foobj\%%~ni.obj %%i
    )
)
set src_dependency_list=
goto:eof




::DIRECTORIES
:bin
if not exist bin (
    md bin
)
goto:eof
:lib
if not exist lib (
    md lib
)
goto:eof
:obj
if not exist obj (
    md obj
)
goto:eof




::UTILITIES
:clean
set del_list=bin lib obj
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