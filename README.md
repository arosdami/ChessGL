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
- **Fordító:** GCC (MinGW-w64 UCRT)

## Funkciók

- Modern, sötét színvilágú főmenü
- Kétjátékos mód (ugyanazon a gépen, felváltva lépve)
- Gép elleni mód (minimax algoritmus alpha-béta vágással)
- Teljes sakk-szabályrendszer
- Kijelölt bábu és a lehetséges lépések vizuális jelölése
- Egérkezelés
- Teljes képernyős mód (F11)
- Saját sakkfigura-ikon az EXE fájlon
- Easter egg

## Fordítás és futtatás

### Előkészületek

- **MinGW-w64 GCC** (javasolt: MSYS2 UCRT64 verzió)
- **GLFW 3.5** fejléc- és könyvtárfájlok (a repóban megtalálhatók)
- **GLAD** (a repóban megtalálható)

### Fordítás

Dupla kattints a **build_and_run.bat** fájlra, vagy PowerShell-ből:

    .\build_and_run.bat

Ez lefordítja a projektet, majd elindítja a **ChessGL.exe**-t.

### Csak fordítás (futtatás nélkül)

    .\build.bat

### Futtatás

    .\run.bat

## Irányítás

| Gomb | Funkció |
|------|---------|
| **Bal egér gomb** | Bábu kiválasztása, majd célmező kattintása |
| **ESC** | Vissza a főmenübe / Kilépés |
| **F11** | Teljes képernyő ki/be |
| **E** | Easter egg be/ki |

## Játékmenet

1. Indítás után a **főmenüben** vagy.
2. Válaszd az **„Új játék (2 játékos)"** vagy a **„Gép ellen"** opciót.
3. A sakktáblán kattints a saját bábúdra – a lehetséges lépések zölddel kiemelődnek.
4. Kattints a célmezőre a lépés végrehajtásához.
5. A játék addig tart, amíg valamelyik fél mattot nem kap, vagy patt nem lesz.

## Projekt struktúra

    ChessGL/
    |-- ChessGL.exe            (a lefordított program)
    |-- glfw3.dll              (GLFW dinamikus könyvtár)
    |-- build.bat              (fordító script)
    |-- build_and_run.bat      (fordítás + futtatás)
    |-- run.bat                (csak futtatás)
    |-- README.md
    |
    |-- include/               (fejléc fájlok)
    |-- lib/                   (GLFW statikus könyvtárak)
    |-- shaders/               (OpenGL shaderek)
    |-- resources/             (ikon és egyéb erőforrások)
    |-- screenshots/           (képernyőképek)
    +-- src/                   (forrásfájlok)
        |-- main.c             (fő program, fő ciklus, képernyőkezelés)
        |-- tabla.c / tabla.h  (sakk logika, bábuk lépései)
        |-- rajz.c / rajz.h    (OpenGL primitívek, szövegrenderelés)
        |-- menu.c / menu.h    (képernyő-váltás kezelése)
        |-- ai.c / ai.h        (minimax AI alpha-béta vágással)
        +-- glad.c             (GLAD betöltő)

## Fejlesztési megjegyzések

- A program **NEM** használ külső 3D modell fájlokat – minden bábu egyszerű
  geometriai primitívumokból (kör, négyzet, háromszög) épül fel.
- A **szövegrenderelés** egy saját 5×7-es bitmap fonttal történik.
- A **shaderek** külön fájlban vannak (shaders/ mappa).
- Az **AI** a minimax algoritmust használja alpha-béta vágással.

## Ismert korlátozások

- Nincs sáncolás (castling) és en passant támogatás
- Az AI nem veszi észre a közvetlen matt-veszélyt, csak az anyagi előnyt

## GitHub

https://github.com/arosdami/ChessGL

## Licenc

Ez a projekt egyetemi bemutató céljából készült. Szabadon felhasználható
tanulási célokra.