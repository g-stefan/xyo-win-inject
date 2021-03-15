@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

rem --- clean

echo -^> clean xyo-win-inject

if exist output\ rmdir /Q /S output
if exist temp\ rmdir /Q /S temp

if exist .vscode\ipch\ rmdir /Q /S .vscode\ipch
