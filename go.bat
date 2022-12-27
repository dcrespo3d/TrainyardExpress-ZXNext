python update_build_version.py

call compile.bat

@if ERRORLEVEL 1 exit /b 1

dir build\trainyard-express_CODE.bin

call run.bat

