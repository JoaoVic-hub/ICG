@echo off
rem Compila os exercicios com o gcc do MSYS2 (MINGW64).
rem Precisa do freeglut instalado no MSYS2:
rem   pacman -S mingw-w64-x86_64-freeglut

set PATH=C:\msys64\mingw64\bin;%PATH%

gcc light_fontes.c -o light_fontes.exe -lfreeglut -lglu32 -lopengl32
gcc movelight_trans.c -o movelight_trans.exe -lfreeglut -lglu32 -lopengl32
gcc movelight_camera.c -o movelight_camera.exe -lfreeglut -lglu32 -lopengl32
gcc light_shade.c -o light_shade.exe -lfreeglut -lglu32 -lopengl32
gcc light_rgb.c -o light_rgb.exe -lfreeglut -lglu32 -lopengl32

rem copia a dll para os .exe abrirem com dois cliques
copy /Y C:\msys64\mingw64\bin\libfreeglut.dll . > nul
