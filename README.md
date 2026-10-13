<div align="center">
  <img src="screenshots/logo.png" alt="ChessGL Logo" width="600" />

  ### Egy modern, C nyelven íródott sakkjáték OpenGL 3.3 Core Profile alapokon.
  
  <br />

  [![GitHub repo](https://img.shields.io/badge/GitHub-Repo-181717?style=for-the-badge&logo=github)](https://github.com/arosdami/ChessGL)
  [![Download release](https://img.shields.io/badge/Release-Download-2ea44f?style=for-the-badge)](#)
  [![Build and run](https://img.shields.io/badge/Build-Run-0A66C2?style=for-the-badge)](#-telep%C3%ADt%C3%A9s-%C3%A9s-ford%C3%ADt%C3%A1s)
  [![How to play](https://img.shields.io/badge/How-to-Play-FFB000?style=for-the-badge)](#-j%C3%A1t%C3%A9kmenet)
</div>

<br />

A **ChessGL** egy egyetemi bemutató és játékfejlesztési feladat eredményeként született projekt. A sakktábla, a menü és a figurák renderelése saját írású shaderekkel és OpenGL 3.3 grafikai motorral történik. A játék saját sakk-logikával és gép elleni (AI) játékmóddal is rendelkezik.

## 📋 Tartalomjegyzék
- [Jellemzők](#-jellemzők)
- [Technológiák](#-technológiák)
- [Telepítés és Fordítás](#-telepítés-és-fordítás)
- [Irányítás](#-irányítás)
- [Játékmenet](#-játékmenet)
- [Képernyőképek](#-képernyőképek)
- [Projekt Struktúra](#-projekt-struktúra)
- [Fejlesztői Megjegyzések](#-fejlesztői-megjegyzések)
- [Készítők & Licenc](#-készítők--licenc)

---

## ✨ Jellemzők

- ♟️ **Kétjátékos mód:** Lokális multiplayer ugyanazon a gépen, felváltott lépésekkel.
- 🤖 **AI ellenfél:** Beépített mesterséges intelligencia három nehézségi szinttel (minimax algoritmus alfa-béta vágással).
- 🧠 **Teljes sakklogika:** Normál lépések, ütésvizsgálat, gyalogátváltozás, valamint sakk-, matt- és patt felismerés.
- 🎨 **Modern megjelenés:** Sötét témájú, letisztult grafikus felület és főmenü.
- 🖼️ **Erőforrás-optimalizált:** Szöveg és figurák renderelése sprite sheet textúrákból és saját bitmap fontokkal.
- 🖥️ **Felhasználói élmény:** Kijelölt bábuk és lehetséges lépések vizuális jelölése, teljes képernyős mód (`F11`), egyedi ablak- és futtatható ikon.

## 🛠 Technológiák

| Kategória | Használt Technológia |
| :--- | :--- |
| **Programozási nyelv** | C (C11 szabvány) |
| **Grafikai API** | OpenGL 3.3 Core Profile |
| **Ablakkezelés & Input** | GLFW 3.x |
| **Bővítmény betöltő** | GLAD |
| **Képbetöltés** | stb_image.h |
| **Fordító (Compiler)** | GCC / MinGW-w64 (Windows) |

---

## 🚀 Telepítés és Fordítás

A projekt Windows rendszeren készült és fut, fordításához **MinGW-w64 GCC** szükséges. A repository már tartalmazza az előre lefordított GLFW statikus könyvtárakat és a GLAD fájlokat.

### Manuális fordítás terminálból

Nyiss meg egy PowerShell vagy parancssor (CMD) ablakot a projekt gyökerében, és futtasd az alábbi parancsokat:

1. **Ikon fordítása (opcionális):**
   ```bat
   windres resources\ikon.rc -O coff -o resources\ikon.o