@echo off

if ["%BOOST_HOME%"] == [] (set BOOST_HOME_ORIGINAL=) else (set BOOST_HOME_ORIGINAL=%BOOST_HOME%)
set BOOST_HOME=%ProgramFiles%\boost\boost_1_73_0
if ["%OPENSSL_HOME%"] == [] (set OPENSSL_HOME_ORIGINAL=) else (set OPENSSL_HOME_ORIGINAL=%OPENSSL_HOME%)
set OPENSSL_HOME=%ProgramFiles%\openssl\openssl-3.0.0-alpha6

path | findstr /C:"Microsoft Visual Studio" > nul || (
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
)
set INCLUDE_ORIGINAL=%INCLUDE%
set INCLUDE=%INCLUDE%;%BOOST_HOME%
if [%1] == [main] (
    call:Main.exe
) else if [%1] == [clean] (
    call:clean
) else (
    call:error
)
if ["%BOOST_HOME_ORIGINAL%"] == [] (set BOOST_HOME=) else (set BOOST_HOME=%BOOST_HOME_ORIGINAL%)
set BOOST_HOME_ORIGINAL=
if ["%OPENSSL_HOME_ORIGINAL%"] == [] (set OPENSSL_HOME=) else (set OPENSSL_HOME=%OPENSSL_HOME_ORIGINAL%)
set OPENSSL_HOME_ORIGINAL=

set INCLUDE=%INCLUDE_ORIGINAL%
set INCLUDE_ORIGINAL=
goto done


::EXECUTABLES
:Main.exe
set obj_dependency_list=obj\driver.obj
set lib_dependency_list=Network.lib GenericIO.lib libcrypto.lib libssl.lib libboost_date_time-vc142-mt-x64-1_73.lib libboost_regex-vc142-mt-x64-1_73.lib
call:bin
if not exist bin\Main.exe (
    call:Main_exe_obj
    call:Main_exe_lib
    link /machine:x64 /libpath:"%cd%\lib" /libpath:"%BOOST_HOME%\stage\lib" /libpath:"%OPENSSL_HOME%" /out:bin\Main.exe %obj_dependency_list% %lib_dependency_list%
    for %%i in (libcrypto-3.dll libssl-3.dll) do (
        copy /Y "%OPENSSL_HOME%\%%i" .\bin\
    )
)
set obj_dependency_list=
set lib_dependency_list=
goto:eof
:Main_exe_obj
set src_dependency_list=src\Sources\driver.cpp
call:obj
for %%i in (%src_dependency_list%) do (
    if not exist obj\%%~ni.obj (
        cl /c /EHsc /Od /MD /I"%cd%\src\Headers" /I"%cd%\src\Headers\GenericIO" /I"%cd%\src\Headers\Network" /I"%BOOST_HOME%" /I"%OPENSSL_HOME%\include" /D_WIN32_WINNT=0x0601 /Foobj\%%~ni.obj %%i
    )
)
set src_dependency_list=
goto:eof
:Main_exe_lib
set lib_dependency_list=lib\Network.lib lib\GenericIO.lib
for %%i in (%lib_dependency_list%) do (
    call:%%i
)
set lib_dependency_list=
goto:eof



:lib\GenericIO.lib
set obj_dependency_list=obj\GenericIO\GenericIStreamBuf.obj^
 obj\GenericIO\GenericStreamBuf.obj
call:lib
if not exist lib\GenericIO.lib (
    call:GenericIO_lib_obj
    lib /out:lib\GenericIO.lib %obj_dependency_list%
)
set obj_dependency_list=
goto:eof
:GenericIO_lib_obj
set src_dependency_list=src\Sources\GenericIO\GenericIStreamBuf.cpp^
 src\Sources\GenericIO\GenericStreamBuf.cpp
for %%i in (%src_dependency_list%) do (
    if not exist obj\GenericIO\%%~ni.obj (
        cl /c /EHsc /Od /MD /I"%cd%\src\Headers\GenericIO" /Foobj\GenericIO\%%~ni.obj %%i
    )
)
set src_dependency_list=
goto:eof

:lib\Network.lib
set obj_dependency_list=obj\Network\HttpByteContent.obj^
 obj\Network\HttpContent.obj^
 obj\Network\HttpFileStreamContent.obj^
 obj\Network\HttpRequestMessage.obj^
 obj\Network\HttpResponseMessage.obj^
 obj\Network\HttpStreamContent.obj^
 obj\Network\HttpStringContent.obj^
 obj\Network\WebClient.obj
call:lib
if not exist lib\Network.lib (
    call:Network_lib_obj
    lib /out:lib\Network.lib %obj_dependency_list%
)
goto:eof
:Network_lib_obj
set src_dependency_list=src\Sources\Network\HttpByteContent.cpp^
 src\Sources\Network\HttpContent.cpp^
 src\Sources\Network\HttpFileStreamContent.cpp^
 src\Sources\Network\HttpRequestMessage.cpp^
 src\Sources\Network\HttpResponseMessage.cpp^
 src\Sources\Network\HttpStreamContent.cpp^
 src\Sources\Network\HttpStringContent.cpp^
 src\Sources\Network\WebClient.cpp
call:obj
for %%i in (%src_dependency_list%) do (
    if not exist obj\%%~ni.obj (
        cl /c /EHsc /Od /MD /I"%cd%\src\Headers\Network" /I"%cd%\src\Headers\GenericIO" /I"%BOOST_HOME%" /I"%OPENSSL_HOME%\include" /D_WIN32_WINNT=0x0601 /Foobj\Network\%%~ni.obj %%i
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
if not exist obj\GenericIO (
    mkdir obj\GenericIO
)
if not exist obj\Network (
    mkdir obj\Network
)
goto:eof




::UTILITIES
:clean
set del_list=bin lib obj\GenericIO obj\Network
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