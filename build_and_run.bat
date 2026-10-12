@echo off
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

echo.
echo ============================================
echo   ChessGL - Forditas
echo ============================================
echo.

REM --- Ha fut a ChessGL, allitsuk le ---
taskkill /F /IM ChessGL.exe >NUL 2>&1
timeout /t 1 /nobreak >NUL

REM --- Ikon forditasa (ha van) ---
set "IKON_OBJ="
if exist resources\sakk.ico (
    if exist resources\ikon.rc (
        echo [1/2] Ikon forditasa...
        windres resources\ikon.rc -O coff -o resources\ikon.o
        if errorlevel 1 (
            echo *** HIBA az ikon forditaskor! ***
            pause
            exit /b 1
        )
        set "IKON_OBJ=resources\ikon.o"
    )
)

REM --- Program forditasa ---
echo [2/2] Program forditasa...
gcc -Wall -Wextra -std=c11 -Iinclude -Isrc ^
    src\main.c src\tabla.c src\rajz.c src\menu.c src\ai.c src\babu.c src\glad.c src\stb_impl.c ^
    %IKON_OBJ% ^
    -o ChessGL.exe ^
    -Llib -lglfw3 -lopengl32 -lgdi32 -lm

if errorlevel 1 (
    echo.
    echo *** HIBA a forditaskor! ***
    pause
    exit /b 1
)

echo.
echo ============================================
echo   SIKERES!  -^>  ChessGL.exe
echo ============================================
echo.
echo Inditas...
echo.

start "" ChessGL.exe