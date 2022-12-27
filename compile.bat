@echo off
rem https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_02_HelloWorld.md

rem -startup=0 (crt0) 32x24 mode, no control code support
rem -startup=1 (crt1) 32x24 mode, control codes supported
rem -startup=4 (crt4) 64x24 mode, no control code support
rem -startup=5 (crt5) 64x24 mode, control codes supported
rem -startup=31 (crt31) stdio not supported at all
rem -startup=8 (crt8) fzx proportional fonts, no control code support
rem -startup=9 (crt9) fzx proportional fonts, control codes supported
rem -startup=30 (crt30) 32x24 rom print routine, no scanf much smaller than other crts because the rom is used for output

@echo on

del build\trainyard*.*

zcc +zxn -subtype=nex -v --list -m -s -clib=new -a page0.c -o page0.asm
@if ERRORLEVEL 1 exit /b 1

python compile_p0.py

zcc +zxn -subtype=nex -v --list -m -s -clib=new -a page1.c -o page1.asm
@if ERRORLEVEL 1 exit /b 1

python compile_p1.py

zcc +zxn -subtype=nex -v --list -m -s -clib=new -startup=31 -pragma-include:zpragma.inc @trainyard.lst -o build/trainyard-express -create-app
@if ERRORLEVEL 1 exit /b 1

exit /b 0
