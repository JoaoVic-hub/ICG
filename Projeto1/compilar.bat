@echo off
rem Compila os exercicios com o gcc do MSYS2 (MINGW64).
rem Precisa do freeglut instalado no MSYS2:
rem   pacman -S mingw-w64-x86_64-freeglut

set PATH=C:\msys64\mingw64\bin;%PATH%

gcc cube_ex1_1.c -o cube_ex1_1.exe -lfreeglut -lglu32 -lopengl32
gcc cube_ex1_2.c -o cube_ex1_2.exe -lfreeglut -lglu32 -lopengl32
gcc robot.c -o robot.exe -lfreeglut -lglu32 -lopengl32
