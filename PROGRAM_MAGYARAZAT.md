# ChessGL - teljes programmagyarázat

Ez a dokumentum a projekt bemutatásához készült. Nem általános sakkleírás,
hanem a program tényleges felépítését magyarázza el: melyik fájl miért van,
hogyan jut el egy egérkattintás a végrehajtott lépésig, és hogyan kapcsolódik
össze a C, a GLFW és az OpenGL.

## 1. A program egy mondatban

A ChessGL egy C11-ben írt, OpenGL 3.3-at használó sakkprogram, amelyben a
`main.c` vezérli a fő ciklust, a `tabla.c` ellenőrzi és hajtja végre a
sakk-lépéseket, a `rajz.c` rajzol, a `menu.c` a képernyőállapotot tárolja,
a `babu.c` a figurákat jeleníti meg, az `ai.c` pedig minimax algoritmussal
választ lépést.

## 2. A legfontosabb fogalmak C-ben

### 2.1 Fordítási egység, fejléc és `#include`

- A `.c` fájl tartalmazza a tényleges függvényimplementációkat.
- A `.h` fájl deklarációkat, típusokat és konstansokat tartalmaz.
- A `#include "tabla.h"` bemásolja a fejléc deklarációit a fordításkor.
- A `#include <...>` külső vagy rendszerkönyvtár fejlécét keresi.
- A `#define` szöveges fordítás előtti helyettesítés. Például a `MERET`
  mindenhol `8` lesz.
- A `#ifndef ... #define ... #endif` fejlécőr: megakadályozza, hogy ugyanazt
  a fejlécet többször dolgozza fel a fordító.

### 2.2 Típusok

```c
typedef enum { URES, GYALOG, BASTYA, HUSZAR, FUTO, KIRALYNO, KIRALY } BabuTipus;
```

Az `enum` névhez egész számot rendel. Az `URES` értéke 0, ezért könnyen
ellenőrizhető, hogy egy mező üres-e.

```c
typedef struct {
    BabuTipus tipus;
    Szin szin;
} Babu;
```

A `struct` több adatot fog össze. Egy `Babu` egyszerre tárolja a figura
típusát és színét.

```c
JatekAllapot *j
```

A `*` itt mutatót jelent. A függvény nem másolatot kap a játékról, hanem az
eredeti `JatekAllapot` memóriacímét, így módosítani tudja azt.

```c
j->tabla[sor][oszlop]
```

A `->` mutatón keresztüli struktúramező-elérés. Ha nem mutató, hanem közvetlen
struktúraváltozó lenne, akkor pontot használnánk: `jatek.kovetkezo`.

### 2.3 Tömbök és indexelés

```c
Babu tabla[8][8];
```

Ez egy kétdimenziós tömb. A program belső koordinátája:

- sor `0` = a fekete kezdő hátsó sora;
- sor `7` = a fehér kezdő hátsó sora;
- oszlop `0` = `a`;
- oszlop `7` = `h`.

Ezért jelenik meg a rajzolásnál a sorok felirata fordítva:
`{"8","7","6","5","4","3","2","1"}`.

### 2.4 `static`, `const`, `void` és visszatérési érték

- A fájlszintű `static` változó vagy függvény csak az adott `.c` fájlból
  látható. Ez védi a modul belső állapotát.
- A `const` értéket a függvény nem módosítja.
- A `void` azt jelenti, hogy a függvény nem ad vissza értéket.
- Az `int` visszatérés általában sikerességet is jelez: `1` sikeres/igaz,
  `0` sikertelen/hamis.

### 2.5 A vezérlési szerkezetek

- `if (...)` feltételes végrehajtás.
- `else` az ellenkező ág.
- `for` ismétlés ismert ciklusszámmal.
- `while` ismétlés feltételig.
- `switch` egy érték több lehetséges ágát kezeli.
- `return` kilép a függvényből, és visszaadja az eredményt.
- `continue` a ciklus következő körére ugrik.

### 2.6 Összetett C-kifejezések

- `&&`: és, mindkét feltétel igaz.
- `||`: vagy, legalább az egyik igaz.
- `!`: logikai negáció.
- `?:`: rövid feltételes kifejezés. Például
  `(s == FEHER) ? -1 : 1`.
- `++`: eggyel növel.
- `abs(...)`: abszolútérték.
- `(Babu){URES, NINCS_SZIN}`: összetett literál, egy üres bábu létrehozása.
- `sizeof(tomb)`: bájtban mért méret.
- `&valtozo`: a változó memóriacíme, kimeneti paraméternél használatos.

## 3. A fájlok szerepe

| Fájl | Feladat |
|---|---|
| [`src/main.c`](./src/main.c) | Inicializálás, főciklus, képernyők, kattintások és billentyűk |
| [`src/tabla.h`](./src/tabla.h) | Sakk-adattípusok és publikus függvénydeklarációk |
| [`src/tabla.c`](./src/tabla.c) | Tábla, szabályos lépés, sakk, matt, patt |
| [`src/rajz.h`](./src/rajz.h) | Grafikai API deklarációi |
| [`src/rajz.c`](./src/rajz.c) | GLFW/OpenGL inicializálás és 2D rajzoló primitívek |
| [`src/babu.h`](./src/babu.h) | Figurák rajzolási felülete |
| [`src/babu.c`](./src/babu.c) | Sprite sheet koordináták és figura-rajzolás |
| [`src/menu.h`](./src/menu.h) | Képernyő- és játékmód-enumerációk |
| [`src/menu.c`](./src/menu.c) | Aktuális képernyő és játékmód tárolása |
| [`src/ai.h`](./src/ai.h) | AI publikus függvényei |
| [`src/ai.c`](./src/ai.c) | Anyagértékelés, minimax és alpha-beta vágás |
| [`src/glad.c`](./src/glad.c) | OpenGL függvények betöltésének implementációja |
| [`src/stb_impl.c`](./src/stb_impl.c) | Az `stb_image` könyvtár egyetlen implementációs példánya |
| [`shaders/vertex.glsl`](./shaders/vertex.glsl) | Vertexek képernyőre transzformálása |
| [`shaders/fragment.glsl`](./shaders/fragment.glsl) | Pixel színe vagy textúrája |

Az `include/`, `lib/` és `resources/` külső függőségeket, képeket és
fordítási erőforrásokat tartalmaz. Ezek nem a sakk szabályait valósítják meg.

## 4. A teljes futási folyamat

### 4.1 Indulás

1. A `main()` meghívja a `rajz_inicializalas()` függvényt.
2. A GLFW létrehozza az ablakot és az OpenGL 3.3 Core kontextust.
3. A GLAD betölti az OpenGL függvénycímeket.
4. A vertex- és fragment-shader betöltődik, majd programba linkelődik.
5. Létrejön a VAO és a VBO.
6. A menü alapállapotba kerül.
7. Betöltődik a `babuk.png` és a készítő képe.
8. A `tabla_inicializalas()` felállítja a kezdő sakkállást.

### 4.2 A főciklus

Amíg az ablakot nem zárták be:

1. Lekérdezi a `menu_aktualis()` értékét.
2. Az aktuális képernyőt kirajzolja.
3. Feldolgozza az egér- és billentyűeseményeket.
4. AI módban fekete következésekor meghívja az AI-t.
5. Meghívja a `rajz_eszkozok_cserel()` függvényt, amely kicseréli a
   front/back buffert és feldolgozza a GLFW eseményeit.

Ez a klasszikus játékciklus: **input -> állapotfrissítés -> renderelés**.

### 4.3 Egy emberi lépés útja

1. A felhasználó bal egérgombbal rákattint egy saját figurára.
2. `jatek_kattintas()` beállítja a kiválasztott sor és oszlop értékét.
3. A következő képkockán `lepesek_listazasa()` minden célmezőre meghívja
   a `babu_szabalyos_lepes()` függvényt.
4. A szabályos célmezők zölddel jelennek meg.
5. A felhasználó egy célmezőre kattint.
6. `lepes_vegrehajtas()` újra ellenőrzi a lépést, majd módosítja a táblát.
7. Megfordul a `kovetkezo` szín.
8. A program ellenőrzi, van-e még legális lépés; így felismeri a mattot vagy
   a pattot.

Az újbóli ellenőrzés fontos: a zöld jelölés csak felhasználói segítség,
nem biztonsági garancia.

## 5. [`tabla.h`](./src/tabla.h): a játék modellje

### 5.1 Enumerációk

`BabuTipus` a hat sakkfigurát és az üres mezőt jelenti. A `Szin` a fehéret,
a feketét és a szín nélküli üres állapotot jelenti.

### 5.2 `JatekAllapot`

Ez a program központi állapota:

- `tabla[8][8]`: minden mező bábuja;
- `kovetkezo`: ki jön;
- `kivalasztott_sor`, `kivalasztott_oszlop`, `van_kivalasztva`: UI-kiválasztás;
- `jatek_vege`: `0` folyamatban, `1` matt, `2` patt;
- `utolso_lepes[32]`: képernyőn megjelenő rövid lépésleírás;
- király- és bástya-mozgásjelzők: sáncolás engedélyezéséhez;
- `en_passant_sor`, `en_passant_oszlop`: az előző dupla gyaloglépés
  után támadható mező.

### 5.3 Publikus függvények

- `tabla_inicializalas`: kezdőállapot.
- `babu_szabalyos_lepes`: egy konkrét lépés validálása.
- `lepes_vegrehajtas`: validálás után valódi állapotmódosítás.
- `lepesek_listazasa`: egy kiválasztott figura összes célmezője.
- `van_legalis_lepes`: van-e legalább egy lépés a színnek.
- `sakkban_van_e`: támadja-e ellenfél a király mezőjét.

## 6. [`tabla.c`](./src/tabla.c): a sakklogika részletesen

### 6.1 Segédfüggvények

- `ellenfel()` a fehérből feketét, feketéből fehéret képez.
- `ervenyes()` megakadályozza a 0..7 tartományon kívüli indexelést.
- `ures_mezot_ellenoriz()` az ürességet vizsgálja.
- `sajat_babu_e()` és `ellenfel_babu_e()` a mezőn álló bábu színét vizsgálja.

### 6.2 `tabla_inicializalas`

Először minden mezőt `(URES, NINCS_SZIN)` értékre állít. A `hatso` tömb
megadja a kezdő hátsó sor sorrendjét:

`bástya, huszár, futó, királynő, király, futó, huszár, bástya`.

Ezután a 0/1 sorra fekete, a 6/7 sorra fehér figurák kerülnek. A többi
változó is nullázódik, így egy új játék nem örököl régi kiválasztást,
sáncolási jogot vagy en passant állapotot.

### 6.3 Gyalog

Fehérnél az irány `-1`, feketénél `+1`, mert a sorindex lefelé növekszik.

- előre egyet csak üres mezőre;
- kezdőhelyről előre kettőt, ha mindkét mező üres;
- átlósan csak ellenfél bábujára üt;
- en passant esetén az előző dupla lépés által kijelölt mezőre léphet.

### 6.4 Bástya

Csak azonos sorban vagy azonos oszlopban mozoghat. A `ds` és `dob` `-1`,
`0` vagy `1` lépésirányt állít elő. A `while` minden köztes mezőt végignéz,
így a bástya nem ugorhat át másik bábun.

### 6.5 Futó

A sor- és oszlopkülönbség abszolút értékeinek egyeznie kell, különben nem
átló. A köztes mezőket ugyanúgy végignézi, mint a bástya.

### 6.6 Huszár

Az `abs` különbségeknek `2+1` vagy `1+2` kombinációt kell adniuk. A huszár
átugorhat más figurákat, ezért nincs köztes mezővizsgálat.

### 6.7 Királynő

A bástya- és futómozgás logikájának OR-kapcsolata. Ez azért működik, mert a
királynő pontosan a két figura mozgását egyesíti.

### 6.8 Király és sáncolás

A normál királylépés legfeljebb egy mező bármely irányban. Saját figurára
nem léphet. Két oszlopnyi elmozdulás sáncolás:

- rövid: `e -> g`, a bástya `h -> f`;
- hosszú: `e -> c`, a bástya `a -> d`.

A kód ellenőrzi, hogy a király és az érintett bástya nem mozdult-e, a
köztes mezők üresek-e, és a sarokban tényleg saját bástya áll-e.

### 6.9 Sakkvizsgálat

`kiraly_helye()` megkeresi a királyt. `mezot_tamadja()` végigjárja a támadó
szín figuráit, és figura szerint kiszámolja, támadható-e a király mezője.
Gyalognál csak az átlós támadás számít; nem használható az előrelépési
szabály.

### 6.10 `babu_szabalyos_lepes`

Ez a legfontosabb központi validátor:

1. határon belüli koordinátákat kér;
2. nem enged üres mezőről lépni;
3. csak a `kovetkezo` szín mozoghat;
4. nem enged saját figurára lépni;
5. meghívja a megfelelő figura-specifikus szabályt;
6. ideiglenesen végrehajtja a lépést;
7. ellenőrzi, hogy a saját király nem került-e sakkba;
8. visszaállítja az eredeti állapotot.

Ez a „szimulálás, ellenőrzés, visszaállítás” minta akadályozza meg például,
hogy egy játékos sakkban maradó lépéssel meneküljön.

### 6.11 `lepes_vegrehajtas`

Ez már nem csak vizsgál, hanem véglegesít:

- en passant esetén leveszi az átugrott gyalogot;
- sáncoláskor átteszi a bástyát;
- utolsó soron a gyalogot automatikusan királynővé változtatja;
- rögzíti, hogy király vagy bástya mozdult;
- átírja a forrás- és célmezőt;
- beállítja a következő en passant mezőt dupla gyaloglépésnél;
- elkészíti az `utolso_lepes` szöveget;
- színt vált;
- ellenőrzi a mattot vagy pattot;
- törli a kiválasztást.

### 6.12 Legális lépések

`lepesek_listazasa()` először lenullázza a 8x8-as jelölőtömböt, majd minden
célmezőt kipróbál. `van_legalis_lepes()` ugyanezt teszi az összes saját
figurával, de az első talált lépésnél visszatér. Ezért a matt/patt vizsgálat
nem számol feleslegesen minden lehetőséget.

## 7. [`menu.c`](./src/menu.c): állapotgép

### 7.1 Képernyőállapotok

`Kepernyo` értékei:

- `KEPERNYO_FOMENU`;
- `KEPERNYO_JATEK`;
- `KEPERNYO_NEVJEGY`;
- `KEPERNYO_KILEPES`;
- `KEPERNYO_AI_NEHEZSEG`.

A `menu.c` két fájlszintű `static` változóban tárolja az aktuális képernyőt
és játékmódot. A lekérdező és beállító függvények miatt a `main.c` nem fér
közvetlenül hozzá ezekhez az adatokhoz.

Ez egy egyszerű állapotgép: a `menu_valtas()` állapotot vált, a `main()`
pedig az aktuális állapot alapján másik rajzoló és inputágat futtat.

## 8. [`main.c`](./src/main.c): felhasználói felület és programvezérlés

### 8.1 Makrók

A fájl eleji `SZIN_...` makrók RGB szín-hármasok. A méretmakrók a gombok,
címek, panelek és szövegek méretét központosítják. Ha például a gombmagasság
változik, elég a `GOMB_H` értéket módosítani.

### 8.2 UI-segédek

- `eger_benne()` eldönti, hogy a kurzor egy téglalapban van-e.
- `eger_kattintott()` csak a lenyomás pillanatában jelez, nem minden
  képkockán át, amikor a gomb nyomva marad.
- `szoveg_kozepre()` a szöveg szélessége alapján középre igazít.
- `gomb_rajzol_ui()` megrajzolja a gombot, hover állapotot kezel, és
  visszaadja, hogy az egér fölötte van-e.

### 8.3 Főmenü és egyéb képernyők

`fomenu_rajzol()` dinamikusan középre helyezi a címet és a gombokat. A
„Folytatás” csak akkor aktív, ha van folytatható játék.

`ai_nehezseg_rajzol()` három nehézséget jelenít meg. A kattintás az
`ai_nehezseg_beallit(i + 1)` hívással állítja be az 1..3 mélységet.

`nevjegy_rajzol()` a készítő adatait és a kör alakú képet jeleníti meg.
`kilepes_rajzol()` csak a megerősítő szöveget rajzolja; az `I` és `N`
billentyűk tényleges feldolgozása a főciklusban történik.

### 8.4 Játéktér

`panelek_szamol()` a bal tábla- és jobb információs panelek méretét számolja.
`tabla_pozicio()` négyzetes táblát illeszt a bal panelbe. A
`jatek_rajzol()`:

1. letörli a hátteret;
2. kirajzolja a panelt;
3. kirajzolja a 64 mezőt sakktábla-színezéssel;
4. zölddel jelöli a legális célokat;
5. sárgával a kiválasztott mezőt;
6. kirajzolja a figurákat;
7. megrajzolja a koordinátákat és a jobb oldali állapotpanelt.

### 8.5 Egérkattintás

`jatek_kattintas()` előbb kizárja az AI fekete körét és a befejezett játékot.
Az egér pixeleiből így lesz táblaindex:

```c
o = (int)((mx - tx) / mezo);
s = (int)((my - ty) / mezo);
```

Első kattintáskor saját figurát választ. Második kattintáskor megpróbálja
végrehajtani a lépést. Hibás célmező esetén ugyanazon a körön belül egy másik
saját figurát is ki lehet választani.

### 8.6 Callbackek és billentyűk

- `ablak_meret_callback()` újraszámolja az OpenGL viewportot.
- `billentyu_callback()` kezeli az ESC, F11, R és E billentyűt.
- ESC játékból/névjegyből/nehézségi menüből főmenübe visz, főmenüből zár.
- F11 vált ablakos és teljes képernyős mód között.
- R új játékot kér.
- E egy apró easter egget kapcsol.

### 8.7 `main()`

A `main()` nem tartalmaz sakk-szabályt. Az összekötő réteg szerepét tölti be:
inicializál, erőforrást tölt, létrehozza a játékállapotot, majd az állapotgép
alapján meghívja a megfelelő modulokat. Ez jó moduláris felépítés, mert a
grafika és a játékszabály külön marad.

## 9. [`ai.c`](./src/ai.c): gépi játékos

### 9.1 Bábértékek

Az AI anyagi kiértékelése:

| Bábu | Pont |
|---|---:|
| Gyalog | 100 |
| Huszár | 320 |
| Futó | 330 |
| Bástya | 500 |
| Királynő | 900 |
| Király | 20000 |

`tabla_ertek()` az AI szempontjából összegzi a saját és az ellenfél figuráit.
Pozitív érték az AI-nak kedvező anyagi állást jelent.

### 9.2 Minimax

Az AI minden saját legális lépést kipróbál. A lépés után a
`minimax()` a következő játékos összes válaszát is vizsgálja:

- AI-körben maximalizálja a pontszámot;
- ellenfél-körben minimalizálja;
- a keresés a beállított mélységnél megáll;
- a leveleken `tabla_ertek()` adja a becslést.

`lepes_szimulal()` csak a táblát módosítja, `lepes_visszaallit()` pedig
visszateszi a levett bábut és visszaállítja a forrásmezőt. Így az AI
kipróbálhatja a lehetőségeket anélkül, hogy tönkretenné az igazi játékot.

### 9.3 Alpha-beta vágás

Az `alfa` az eddig ismert legjobb maximalizáló érték, a `beta` az eddig ismert
legjobb minimalizáló érték. Ha `beta <= alfa`, a további ág már nem tudna
jobb döntést adni, ezért a program eldobja azt. Az eredmény ugyanaz, mint
teljes minimaxnál, de kevesebb állapotot kell megvizsgálni.

### 9.4 Nehézség és korlát

Az 1, 2 és 3 mélység a következő lépések számát korlátozza. A jelenlegi
értékelés főleg anyagi előnyt számol, nem teljes sakkstratégiai értékelés.
Ezért az AI nem feltétlenül találja meg a közvetlen mattot olyan jól, mint
egy teljes sakkmotor.

Fontos megjegyzés: az AI szimulációja a gyors keresés miatt alapvetően
figuramozgatást vizsgál. A valódi lépést a `lepes_vegrehajtas()` hajtja végre,
tehát az emberi játéknál használt teljes állapotfrissítés ott történik meg.

## 10. [`rajz.c`](./src/rajz.c): OpenGL-réteg

### 10.1 Inicializálás

`rajz_inicializalas()`:

1. `glfwInit()`;
2. OpenGL 3.3 Core Profile ablak;
3. aktuális kontextus;
4. GLAD betöltése;
5. shader program létrehozása;
6. VAO/VBO létrehozása;
7. blending bekapcsolása;
8. kezdő viewport.

### 10.2 Vertexformátum

Minden vertex 7 `float`:

```text
x, y, u, v, r, g, b
```

- `x,y`: képernyőkoordináta;
- `u,v`: textúra-koordináta;
- `r,g,b`: szín.

A stride `7 * sizeof(float)`. A három vertexattribútum a shaderben a
0., 1. és 2. locationhöz kapcsolódik.

### 10.3 Képernyő-koordináták

Az ortografikus projekció a pixeleket közvetlenül használható koordinátává
alakítja. Az y-tengely megfordul, így a program bal felső sarokhoz igazodó
UI-koordinátákkal dolgozhat.

### 10.4 Rajzoló primitívek

- `rajz_negyzet()`: két háromszögből álló téglalap.
- `rajz_kor()`: 32 háromszögből közelített kör.
- `rajz_haromszog()`: egy háromszög.
- `csucsok_kuld()`: VBO-ba másol, majd `glDrawArrays(GL_TRIANGLES, ...)`.

### 10.5 Textúrák

`rajz_textura_betolt()` az `stb_image` segítségével képet tölt be, OpenGL
textúrát készít, lineáris szűrést állít be, majd felszabadítja a CPU-oldali
képadatot.

`rajz_textura_negyszog()` sprite-részletet rajzol. A `u0,v0,u1,v1`
paraméterekkel nem kell a teljes képet megjeleníteni.
`rajz_textura_kor()` négyzetet rajzol, a fragment shader pedig körön kívüli
pixeleket eldob.

### 10.6 Bitmap font

A `FONT[96][5]` minden ASCII karakterhez 5 oszlopnyi, 7 bites mintát tárol.
A `karakter_rajzol()` minden bekapcsolt bitből kis négyzetet készít.
Ezért nincs külön font-renderelő könyvtár; a szöveg is OpenGL-geometriából
épül fel.

## 11. [`babu.c`](./src/babu.c): sprite sheet

A `resources/babuk.png` hat oszlopos és két soros kép:

```text
felső sor: fehér figurák
alsó sor: fekete figurák

oszlopok: királynő, király, futó, huszár, bástya, gyalog
```

`babu_cella()` a `Babu` típusából sprite-koordinátát készít.
`babu_rajzol()` a megfelelő UV-részletet rajzolja ki a mező belsejében.
Ha nincs textúra, tartalék megjelenítésként egy színes kört rajzol.
`sziv_rajzol()` két körből és egy háromszögből állít elő szívet a névjegyhez.

## 12. A shaderek

### 12.1 [`vertex.glsl`](./shaders/vertex.glsl)

Az `aPos` a pozíció, `aTex` a textúra-koordináta, `aColor` a szín.
A `projection` mátrix a pozíciót OpenGL clip-space koordinátává alakítja.
Az `out` változók továbbmennek a fragment shaderhez, ahol a háromszög
pixelei között interpolálódnak.

### 12.2 [`fragment.glsl`](./shaders/fragment.glsl)

Ha `hasznal_textura == 1`, a shader a textúrából mintavételez. Az alfaérték
0.02 alatti pixeleket eldobja, így a sprite átlátszó háttere nem látszik.
`kormaszk == 1` esetén a középponttól távoli pixeleket `discard`-olja, így
négyzetes kép helyett kör alakú kép jelenik meg.

Ha nincs textúra, a `Szin` interpolált RGB értékből lesz a pixel színe.

## 13. Build és futtatás

A projekt GCC/MinGW-w64 környezetre készült. A lényeges parancsok:

```bat
windres resources\ikon.rc -O coff -o resources\ikon.o

gcc -mwindows -Wall -Wextra -std=c11 -Iinclude -Isrc ^
    src\main.c src\tabla.c src\rajz.c src\menu.c src\ai.c src\babu.c ^
    src\glad.c src\stb_impl.c resources\ikon.o ^
    -o ChessGL.exe -Llib -lglfw3 -lopengl32 -lgdi32 -lm
```

- `-Wall -Wextra`: figyelmeztetések bekapcsolása.
- `-std=c11`: C11 szabvány.
- `-Iinclude -Isrc`: fejléc-keresési útvonalak.
- `-Llib`: könyvtárkeresési útvonal.
- `-lglfw3`, `-lopengl32`, `-lgdi32`, `-lm`: linkelt könyvtárak.
- `-mwindows`: Windows alatt ne nyíljon külön konzolablak.

Az automatikus buildet a gyökérben lévő `build_and_run.bat` végzi. A
`build_files/build_and_run.bat` régebbi/alternatív script; bemutató előtt a
gyökérbeli scriptet érdemes használni, mert az tartalmazza az AI- és
bábu-modulokat is.

## 14. Mit érdemes elmondani a bemutatón?

Egy rövid, magabiztos bemutatási sorrend:

1. „A program moduláris: külön van a modell, a grafika, a menü és az AI.”
2. „A `JatekAllapot` tartalmazza a teljes sakkállapotot.”
3. „Minden lépés először validálódik, majd szimulációval ellenőrizzük, hogy
   a saját király nem marad-e sakkban.”
4. „A megjelenítés OpenGL-ben történik, a figurák sprite sheetből jönnek.”
5. „A főciklus inputot kezel, állapotot frissít és renderel.”
6. „Az AI minimaxot használ alpha-beta vágással, egyszerű anyagi
   kiértékeléssel.”
7. „A sakkfunkciók között jelenleg a sáncolás, en passant és
   gyalogátváltozás is szerepel.”

## 15. Tipikus tanári kérdések és rövid válaszok

**Miért kell a fejlécfájl?**  
Hogy a modulok ismerjék egymás függvényeinek és típusainak deklarációit,
miközben az implementáció külön `.c` fájlban marad.

**Miért pointert kap a legtöbb sakkfüggvény?**  
Mert ugyanazt a játéktáblát kell olvasniuk és bizonyos esetekben módosítaniuk;
másolattal a változás elveszne.

**Miért kell a lépést szimulálni?**  
Mert egy geometriailag szabályos lépés még illegális lehet, ha utána a saját
király sakkban marad.

**Miért kell a `kovetkezo` mező?**  
Ez biztosítja, hogy csak az aktuális játékos figurája mozoghasson.

**Hogyan lesz egy képkockából kép?**  
A `main` meghívja a megfelelő rajzolófüggvényt, az OpenGL a VBO vertexeit
feldolgozza, a fragment shader pedig minden látható pixel végső színét adja.

**Mit csinál az alpha-beta?**  
Olyan minimax-ágakat hagy el, amelyekről már tudható, hogy nem lehetnek
jobbak az eddig ismert választásnál.

**Miért nem használ a program 3D modellt?**  
A projekt 2D OpenGL-geometriával és textúrázott sprite-okkal rajzolja a
3D-modellezési feladat sakkfelületét; a grafikai API ettől még OpenGL 3.3.

## 16. Egy fontos kódolvasási módszer

Amikor a tanár egy sort kérdez, mindig ebben a sorrendben gondolkodj:

1. Milyen típusú változó ez?
2. Helyi, globális vagy `static`?
3. Értéket olvas vagy módosít?
4. Milyen feltétel/ciklus vezérli?
5. Melyik másik függvény vagy modul használja?
6. Mi történik a következő sorban az állapottal?

Például:

```c
if (lepes_vegrehajtas(j, sr, so, s, o)) {
    ...
}
```

Ez azt jelenti, hogy a program megkísérli a kiválasztott figura célmezőre
mozgatását; a függvény előbb validál, hamis értéknél nem változtat semmit,
igaz értéknél viszont a tábla és a következő játékos állapota már frissült.

