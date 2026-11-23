@echo off
rem Compila os exercicios com o gcc do MSYS2 (MINGW64).
rem Precisa do freeglut instalado no MSYS2:
rem   pacman -S mingw-w64-x86_64-freeglut

set PATH=C:\msys64\mingw64\bin;%PATH%

gcc checker_textura.c -o checker_textura.exe -lfreeglut -lglu32 -lopengl32
gcc textura_imagem.c -o textura_imagem.exe -lfreeglut -lglu32 -lopengl32

rem copia as dlls para os .exe abrirem com dois cliques
rem (o stb_image.h usa thread_local, que no MinGW precisa da libwinpthread)
copy /Y C:\msys64\mingw64\bin\libfreeglut.dll . > nul
copy /Y C:\msys64\mingw64\bin\libwinpthread-1.dll . > nul
