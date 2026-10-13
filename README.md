
<div align="center">
  <img src="screenshots/logo.png" alt="ChessGL logo" width="800" />
</div>

<div align="center">
  <a href="https://github.com/arosdami/ChessGL"><img src="https://img.shields.io/badge/GitHub-Repo-181717?style=for-the-badge&logo=github" alt="GitHub repo" /></a>
  <a href="https://github.com/arosdami/ChessGL/releases"><img src="https://img.shields.io/badge/Release-Download-2ea44f?style=for-the-badge" alt="Download release" /></a>
  <a href="#build--run"><img src="https://img.shields.io/badge/Build-Run-0A66C2?style=for-the-badge" alt="Build and run" /></a>
  <a href="#gameplay"><img src="https://img.shields.io/badge/How_to_Play-FFB000?style=for-the-badge" alt="How to play" /></a>
</div>

<p align="center">
Egy modern, C nyelven íródott sakkjáték, amely OpenGL 3.3 Core Profile grafikai motorral rendereli a sakktáblát, a menüt és a figurákat. A projekt egy egyetemi bemutató és játékfejlesztési feladat eredménye, saját shader-ekkel, saját sakk-logikával és gép elleni játékmóddal.

</p>

## Jellemzők

- Kétjátékos mód ugyanazon a gépen, felváltva lépéssel
- AI ellen játszható mód (minimax + alpha-beta vágás)
- Teljes sakklogika:
  - normál lépések minden bábúhoz
  - ütésvizsgálat
  - gyalogátváltozás
  - sakk, matt és patt felismerés
- Sötét, modern felület és főmenü
- Textúrázott sakkfigurák sprite sheet-ből
- Kijelölt bábu és lehetséges lépések vizuális jelölése
- Teljes képernyős mód (`F11`)
- Saját EXE ikon és ablakikon

![Játéktér](screenshots/sakk.png)

## Technológiák

- Nyelv: C (C11)
- Grafika: OpenGL 3.3 Core Profile
- Ablakkezelés: GLFW 3.x
- OpenGL függvénybetöltés: GLAD
- Képbetöltés: stb_image.h
- Fordító: GCC / MinGW-w64

## Szükséges környezet

A projekt Windows rendszeren készül és fut, MinGW-w64 GCC-vel. A repo már tartalmazza a szükséges GLFW és GLAD fájlokat is.

### Könyvtárak és erőforrások

- `lib/` – GLFW statikus könyvtárak
- `include/` – GLAD és stb_image fejlécfájlok
- `src/` – a játék forráskódja
- `shaders/` – OpenGL vertex/fragment shader fájlok
- `resources/` – ikonok, textúrák, képek
- `screenshots/` – képernyőképek a dokumentációhoz


Ez buildeli a programot, és elindítja a `ChessGL.exe` fájlt.

## Manuális fordítás

Nyisd meg a PowerShellt a projekt gyökerében, majd add ki:

```bat
windres resources\ikon.rc -O coff -o resources\ikon.o

gcc -Wall -Wextra -std=c11 -Iinclude -Isrc ^
    src\main.c src\tabla.c src\rajz.c src\menu.c src\ai.c src\babu.c src\glad.c src\stb_impl.c ^
    resources\ikon.o ^
    -o ChessGL.exe ^
    -Llib -lglfw3 -lopengl32 -lgdi32 -lm
```

Ez létrehozza a fordított futtatható fájlt:

```bat
.\ChessGL.exe
```

Ha csak ikon nélkül akarod fordítani, a `resources\ikon.o` részt elhagyhatod.

## Irányítás

| Gomb | Funkció |
|------|---------|
| Bal egérgomb | Bábu kiválasztása, majd célmező kijelölése / lépés végrehajtása |
| ESC | Vissza a főmenübe vagy kilépés |
| R | Új játék indítása az aktuális módban |
| F11 | Teljes képernyő ki/be |

## Játékmenet

1. Indítás után a főmenüben válaszd az `Új játék` vagy az `AiChess` opciót.
2. Az AI módnál kiválasztható a nehézségi szint: könnyű, normál vagy erős.
3. Kattints a saját bábudra a sakktáblán.
4. A lehetséges lépések zölddel lesznek jelölve.
5. Kattints a célmezőre a lépés végrehajtásához.
6. `ESC` segítségével visszaléphetsz a főmenübe, és a `Folytatás` gombbal visszatérhetsz a játékhoz.
7. A játék addig tart, amíg valamelyik játékos mattot kap, vagy a pozíció pattállásba kerül.

## Projekt struktúra

```text
ChessGL/
├── ChessGL.exe             # Fordított futtatható program
├── glfw3.dll               # GLFW DLL
├── README.md               # Projekt leírás
├── LICENSE                 # Licenc
├── lib/                    # GLFW statikus könyvtárak
├── include/                # GLAD és stb_image fejlécfájlok
├── shaders/                # OpenGL shader fájlok
│   ├── vertex.glsl
│   └── fragment.glsl
├── resources/              # Ikonok, textúrák és képek
│   ├── sakk.ico
│   ├── icon.png
│   ├── babuk.png
│   ├── creator.png
│   ├── ikon.rc
│   └── ikon.o              # Opcionális fordítási objektum
├── screenshots/            # Képernyőképek a dokumentációhoz
│   ├── logo.png
│   ├── mainmenu.png
│   └── sakk.png
├── src/                    # Forrásfájlok
│   ├── main.c
│   ├── tabla.c / tabla.h
│   ├── rajz.c / rajz.h
│   ├── babu.c / babu.h
│   ├── menu.c / menu.h
│   ├── ai.c / ai.h
│   ├── glad.c
│   ├── stb_impl.c
│   └── ...
└── .gitignore
```

## Fejlesztési megjegyzések

- A program nem használ külső 3D modellt, a sakkfigurák egyetlen sprite sheet textúrából töltődnek be.
- A szöveg renderelése saját bitmap fonttal történik.
- A shader fájlok külön mappában vannak (`shaders/`).
- Az AI minimax algoritmust használ alpha-beta vágással.
- A textúrák betöltéséhez a `stb_image.h` header-only könyvtár szolgál.

## Ismert korlátozások

- Nincs sáncolás és en passant támogatás
- Az AI nem mindig veszi észre a közvetlen matt- vagy veszélyhelyzetet, csak az anyagi előnyt számolja

## Készítette

- Név: Aros Damján
- Tantárgy: 3D modellezés és játékfejlesztés
- Oktató: Dr. Mileff Péter

## GitHub

https://github.com/arosdami/ChessGL

## Licenc

Ez a projekt a [MIT License](LICENSE) feltételei alapján kerül kiadásra.

A licenc lehetővé teszi, hogy a szoftvert szabadon használni, másolni, módosítani és terjeszteni lehessen, mindaddig, amíg a szerzői jogi megjegyzés és a licenc szövege megmarad a másolatokban és a módosított változatokban is.

A szoftver „adott állapotában” kerül biztosításra, mindenféle garancia nélkül, beleértve, de nem kizárólag a forgalomképességre, egy adott célra való alkalmasságra és nem sértő jellegre vonatkozó garanciákat.

A teljes szöveg elérhető a [LICENSE](LICENSE) fájlban.

> Copyright (c) 2026 Aros Damján
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.