@echo off

if [%1] == [cpp] (
    goto process_cpp
) else if [%1] == [c] (
    goto process_c
) else (
    echo no language specified
    goto invalid_language
)
goto done

:process_cpp
if [%4] == [] (
    goto error
)
if exist %4 (
    findstr "EXPORTS" %4 >> nul || (
    echo EXPORTS > %4
    )
) else (
    echo EXPORTS > %4
)
for %%a in (%3\*.obj) do (
    dumpbin /out:%3\%%~na.exp /symbols %%a
    for /f %%b in (%2) do (
        for /f "tokens=5,6,7,8" %%c in ('findstr "%%b" %3\%%~na.exp') do (
            if [%%c] == [External] (
                findstr "%%e" %4 > nul || (
                    echo %%e >> %4
                )
            ) else if [%%d] == [External] (
                findstr "%%f" %4 > nul || (
                    echo %%f >> %4
                )
            )
        )
    )
    del %3\%%~na.exp
)
goto done

:process_c
if [%3] == [] (
    goto error
)
if exist %3 (
    findstr "EXPORTS" %3 || (
        echo EXPORTS > %3
    )
) else (
    echo EXPORTS > %3
)
for /f %%b in (%2) do (
    echo %%b >> %3
)
goto done

:invalid_language
echo Invalid language specification

:error
echo Usage: %0 c [input_symbol_file_name] [output_export_def_file_name]
echo Usage: %0 cpp [input_symbol_file_name] [obj_dir] [output_export_def_file_name]

:done
