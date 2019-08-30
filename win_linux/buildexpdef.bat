@echo off

if [%3] == [] (
    goto error
)
echo EXPORTS > %3
for %%a in (%2\*.obj) do (
    dumpbin /out:%2\%%~na.exp /symbols %%a
    for /f %%b in (%1) do (
        for /f "tokens=6,8" %%c in ('findstr "%%b" %2\%%~na.exp') do (
            if [%%c] == [External] (
                findstr "%%d" %3 > nul || (
                    echo %%d >> %3
                )
            )
        )
    )
    del %2\%%~na.exp
)
goto done

:error
echo Usage: %0 [input_symbol_file_name] [obj_dir] [output_export_def_file_name]

:done
