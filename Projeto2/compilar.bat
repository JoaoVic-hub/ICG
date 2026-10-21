@echo off
rem Compila os exercicios com o gcc do MSYS2 (MINGW64).
rem Precisa do freeglut instalado no MSYS2:
rem   pacman -S mingw-w64-x86_64-freeglut

set PATH=C:\msys64\mingw64\bin;%PATH%

gcc cube_vis.c -o cube_vis.exe -lfreeglut -lglu32 -lopengl32
gcc robot_vis.c -o robot_vis.exe -lfreeglut -lglu32 -lopengl32
gcc robot_pega.c -o robot_pega.exe -lfreeglut -lglu32 -lopengl32

rem copia a dll para os .exe abrirem com dois cliques
copy /Y C:\msys64\mingw64\bin\libfreeglut.dll . > nul
