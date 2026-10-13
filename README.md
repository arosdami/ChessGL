![ChessGL Logo](screenshots/logo.png)

# ChessGL – OpenGL Sakkjáték

Egy modern, C nyelven íródott sakkjáték OpenGL 3.3 Core Profile grafikával.
A projekt egy egyetemi bemutató keretein belül készült, saját shaderekkel,
saját sakk-logikával és gép elleni móddal.

## Képernyőképek

### Főmenü

![Főmenü](screenshots/mainmenu.png)

### Játéktér

![Játéktér](screenshots/sakk.png)

## Készítette

- **Név:** Aros Damján
- **Tantárgy:** 3D modellezés és játékfejlesztés
- **Oktató:** Dr. Mileff Péter

## Technológiák

- **Nyelv:** C (C11)
- **Grafika:** OpenGL 3.3 Core Profile – saját vertex- és fragment shader
- **Ablakkezelés:** GLFW 3.5
- **OpenGL függvénybetöltő:** GLAD
- **Képbetöltés:** stb_image (single-header library)
- **Fordító:** GCC (MinGW-w64 UCRT)

## Funkciók

- Modern, sötét színvilágú főmenü
- Kétjátékos mód (ugyanazon a gépen, felváltva lépve)
- Gép elleni mód (minimax algoritmus alpha-béta vágással)
- Teljes sakk-szabályrendszer:
  - Minden bábu szabályos lépése
  - Ütésvizsgálat
  - Gyalogátváltozás
  - Sakk-matt és patt felismerése
- **Textúrázott sakkfigurák** – minden bábu egyedi képpel
- **Saját arckép a névjegyben** – kör alakban megjelenítve
- Kijelölt bábu és a lehetséges lépések vizuális jelölése
- Egérkezelés
- Teljes képernyős mód (F11)
- Saját sakkfigura-ikon az EXE fájlon és az ablakon
- Easter egg

## Fordítás és futtatás

A projekt Windows alatt, MinGW-w64 GCC-vel és GLFW-vel fordítható.

### Szükséges könyvtárak

- **GLFW 3.5** – ablakkezelés és bemenet (a repóban megtalálható a **lib/** mappában)
- **GLAD** – OpenGL függvénybetöltő (a **src/glad.c** és az **include/glad/** mappában)
- **stb_image.h** – PNG-kép betöltő könyvtár (az **include/** mappában)
- **OpenGL** – a rendszer része

### Fordítási folyamat

A program három lépésben épül fel:

1. **Ikon erőforrás fordítása** – a **resources/sakk.ico** ikonból **windres**
   segítségével **ikon.o** objektumfájl készül.
2. **Forrásfájlok fordítása** – a **src/** mappában található összes **.c** fájlt
   a **gcc** lefordítja **.o** objektumfájlokká.
3. **Linkelés** – az objektumfájlokat és a GLFW könyvtárat összefűzi a linker,
   és elkészül a **ChessGL.exe**.

### Fordítás

Dupla kattints a **build_and_run.bat** fájlra, vagy PowerShell-ből:

    .\build_and_run.bat

### Manuális fordítás

    gcc -Wall -Wextra -std=c11 -Iinclude -Isrc ^
        src/main.c src/tabla.c src/rajz.c src/menu.c src/ai.c src/babu.c src/glad.c src/stb_impl.c ^
        resources/ikon.o ^
        -o ChessGL.exe ^
        -Llib -lglfw3 -lopengl32 -lgdi32 -lm

## Irányítás

| Gomb | Funkció |
|------|---------|
| **Bal egér gomb** | Bábu kiválasztása, majd célmező kattintása |
| **ESC** | Vissza a főmenübe / Kilépés |
| **F11** | Teljes képernyő ki/be |
| **E** | Easter egg be/ki |

## Projekt struktúra

    ChessGL/
    |-- ChessGL.exe
    |-- glfw3.dll
    |-- build.bat, build_and_run.bat, run.bat
    |-- README.md
    |-- include/               (fejléc fájlok)
    |-- lib/                   (GLFW statikus könyvtárak)
    |-- shaders/               (OpenGL shaderek)
    |-- resources/             (ikonok, sprite sheet, arckép)
    |-- screenshots/           (képernyőképek)
    +-- src/                   (forrásfájlok)

## GitHub

https://github.com/arosdami/ChessGL

## Licenc

Ez a projekt egyetemi bemutató céljából készült. Szabadon felhasználható
tanulási célokra.