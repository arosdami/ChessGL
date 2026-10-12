#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "rajz.h"
#include "tabla.h"
#include "menu.h"
#include "ai.h"

// ============================================================
//                    GOMBOK A FOMENUBEN
// ============================================================
typedef struct {
    const char *felirat;
    float x, y, w, h;
} Gomb;

static Gomb fomenu_gombok[] = {
    {"Duel", 0, 0, 0, 0},
    {"Gep ellen",            0, 0, 0, 0},
    {"Nevjegy",              0, 0, 0, 0},
    {"Kilepes",              0, 0, 0, 0},
};
static const int FOMENU_GOMB_DB = 4;

static int eger_elozo = 0;
static int kivalasztott_gomb = -1;
static int easter_egg_aktivalva = 0;

// ============================================================
//                    SEGEDFUGGVENYEK
// ============================================================
static int eger_benne(float x, float y, float w, float h) {
    double mx = rajz_eger_x();
    double my = rajz_eger_y();
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

static int eger_kattintott(void) {
    int most = rajz_gomb_lenyomva(GLFW_MOUSE_BUTTON_LEFT);
    int kattintott = (most && !eger_elozo);
    eger_elozo = most;
    return kattintott;
}

// ============================================================
//                    SZIV RAJZOLAS
// ============================================================
static void sziv_rajzol(float cx, float cy, float meret, float r, float g, float b) {
    float fel = meret * 0.5f;
    rajz_kor(cx - fel * 0.55f, cy - fel * 0.20f, fel * 0.55f, r, g, b);
    rajz_kor(cx + fel * 0.55f, cy - fel * 0.20f, fel * 0.55f, r, g, b);
    rajz_haromszog(cx - fel, cy + fel * 0.05f,
                   cx + fel, cy + fel * 0.05f,
                   cx,       cy + fel * 1.30f,
                   r, g, b);
}

// ============================================================
//                       FOMENU
// ============================================================
static void fomenu_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    rajz_torol(0.07f, 0.08f, 0.11f);

    float cim_meret   = 70.0f;
    float alcim_meret = 26.0f;
    float gomb_w      = 360.0f;
    float gomb_h      = 55.0f;
    float gomb_koz    = 16.0f;

    float cim_sav     = cim_meret + 12.0f;
    float alcim_sav   = alcim_meret + 50.0f;
    float gombok_sav  = FOMENU_GOMB_DB * gomb_h + (FOMENU_GOMB_DB - 1) * gomb_koz;
    float blokk_mag   = cim_sav + alcim_sav + gombok_sav;

    float blokk_y = (mag - blokk_mag) / 2.0f;

    const char *cim = "CHESSGL";
    float cim_szel  = rajz_szoveg_szelesseg(cim, cim_meret);
    rajz_szoveg(cim, (szel - cim_szel) / 2.0f, blokk_y,
                cim_meret, 0.95f, 0.85f, 0.55f);

    const char *alcim = "by arosdami";
    float a_szel  = rajz_szoveg_szelesseg(alcim, alcim_meret);
    rajz_szoveg(alcim, (szel - a_szel) / 2.0f, blokk_y + cim_sav,
                alcim_meret, 0.65f, 0.68f, 0.75f);

    float gombok_y = blokk_y + cim_sav + alcim_sav;
    float x = (szel - gomb_w) / 2.0f;

    for (int i = 0; i < FOMENU_GOMB_DB; i++) {
        fomenu_gombok[i].x = x;
        fomenu_gombok[i].y = gombok_y + i * (gomb_h + gomb_koz);
        fomenu_gombok[i].w = gomb_w;
        fomenu_gombok[i].h = gomb_h;

        int folotte = eger_benne(fomenu_gombok[i].x, fomenu_gombok[i].y,
                                 fomenu_gombok[i].w, fomenu_gombok[i].h);

        if (folotte)
            rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, gomb_h,
                         0.20f, 0.28f, 0.42f);
        else
            rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, gomb_h,
                         0.14f, 0.16f, 0.22f);

        rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, 2.0f,
                     folotte ? 0.75f : 0.35f,
                     folotte ? 0.65f : 0.38f,
                     folotte ? 0.35f : 0.50f);

        float t_meret = 22.0f;
        float t_szel  = rajz_szoveg_szelesseg(fomenu_gombok[i].felirat, t_meret);
        rajz_szoveg(fomenu_gombok[i].felirat,
                    x + (gomb_w - t_szel) / 2.0f,
                    fomenu_gombok[i].y + (gomb_h - t_meret) / 2.0f,
                    t_meret,
                    folotte ? 1.0f : 0.85f,
                    folotte ? 1.0f : 0.85f,
                    folotte ? 1.0f : 0.85f);
    }

    const char *lablec = "F11 = teljes kepernyo ki/be     ESC = kilepes";
    float l_meret = 14.0f;
    float l_szel  = rajz_szoveg_szelesseg(lablec, l_meret);
    rajz_szoveg(lablec, (szel - l_szel) / 2.0f, mag - 25.0f,
                l_meret, 0.45f, 0.48f, 0.55f);
}

// ============================================================
//                       NEVJEGY
// ============================================================
static void nevjegy_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    rajz_torol(0.07f, 0.08f, 0.11f);

    // Kis kepernyokre kicsinyites
    float scale = 1.0f;
    if (mag < 900.0f) scale = mag / 900.0f;
    if (scale < 0.55f) scale = 0.55f;

    // ---- Meretek ----
    float cim_meret   = 42.0f * scale;
    float cim_sav     = cim_meret + 30.0f * scale;

    float cimke_m     = 13.0f * scale;
    float ertek_m     = 22.0f * scale;
    float fejlec_par_mag = cimke_m + 6.0f * scale + ertek_m + 20.0f * scale;
    float fejlec_sav  = 3 * fejlec_par_mag;

    float leiras_cim_m    = 16.0f * scale;
    float leiras_szoveg_m = 14.0f * scale;
    float leiras_sor_koz  = 20.0f * scale;

    const char *leiras[] = {
        "A ChessGL egy klasszikus sakkjatek, C nyelven,",
        "modern OpenGL 3.3 Core Profile grafikaval.",
        "",
        "A megjelenites teljes egeszeben shader-alapu:",
        "a babuk, a sakktabla es a menuelemek sajat geo-",
        "metriai primitivumokbol epulnek fel, sajat vertex-",
        "es fragment shaderrel, egyetlen VBO-bol kirajzolva.",
        "",
        "A jateklogika sajat implementacio: minden babu",
        "szabalyos lepese, utkozesvizsgalat, gyalogatval-",
        "tozas, valamint a jatek vegso allapotanak felis-",
        "merese.",
        "",
        "A gep elleni mod minimax algoritmuson alapul,",
        "alpha-beta vagassal. 2 jatekos mod is elerheto."
    };
    int leiras_db = sizeof(leiras) / sizeof(leiras[0]);

    // A doboz belso meretei
    float doboz_belso_margo = 30.0f * scale;
    float doboz_felso_margo = 55.0f * scale;
    float doboz_also_margo  = 25.0f * scale;

    float leiras_tartalom_mag = leiras_db * leiras_sor_koz;
    float doboz_mag = doboz_felso_margo + leiras_tartalom_mag + doboz_also_margo;

    float link_sav  = 70.0f * scale;
    float sziv_sav  = 60.0f * scale;

    float blokk_mag = cim_sav + fejlec_sav + 30.0f * scale
                      + doboz_mag + link_sav + sziv_sav;

    // ---- Kozepre igazitas ----
    float blokk_y = (mag - blokk_mag) / 2.0f;
    if (blokk_y < 20.0f) blokk_y = 20.0f;

    // ---- Cim ----
    const char *cim = "NEVJEGY";
    float cim_szel = rajz_szoveg_szelesseg(cim, cim_meret);
    rajz_szoveg(cim, (szel - cim_szel) / 2.0f, blokk_y,
                cim_meret, 0.95f, 0.85f, 0.55f);

    float y = blokk_y + cim_sav;

    // ---- Fejlec: 3 par sor ----
    const char *fejlec_cimkek[] = { "Keszitette:", "Tantargy:", "Oktato:" };
    const char *fejlec_ertekek[] = {
        "Aros Damjan",
        "3D modellezes es jatekfejlesztes",
        "Dr. Mileff Peter"
    };

    for (int i = 0; i < 3; i++) {
        float cw = rajz_szoveg_szelesseg(fejlec_cimkek[i], cimke_m);
        float ew = rajz_szoveg_szelesseg(fejlec_ertekek[i], ertek_m);

        rajz_szoveg(fejlec_cimkek[i], (szel - cw) / 2.0f, y,
                    cimke_m, 0.55f, 0.62f, 0.72f);
        y += cimke_m + 6.0f * scale;

        rajz_szoveg(fejlec_ertekek[i], (szel - ew) / 2.0f, y,
                    ertek_m, 0.95f, 0.92f, 0.78f);
        y += ertek_m + 20.0f * scale;
    }

    // Egy kis hezag
    y += 30.0f * scale;

    // ---- Leiras doboz ----
    // A doboz szelessege: kepernyo 55%-a, de legalabb 500 pixel
    float doboz_w = szel * 0.55f;
    if (doboz_w < 500.0f) doboz_w = 500.0f;
    if (doboz_w > 900.0f) doboz_w = 900.0f;

    // KÖZÉPRE igazítjuk a dobozt
    float doboz_x = (szel - doboz_w) / 2.0f;
    float doboz_y = y;

    // Hatterszin
    rajz_negyzet(doboz_x, doboz_y, doboz_w, doboz_mag,
                 0.12f, 0.14f, 0.19f);
    // Diszkret felso csik
    rajz_negyzet(doboz_x, doboz_y, doboz_w, 2.0f,
                 0.35f, 0.65f, 0.80f);

    // Leiras cim – kozepen
    const char *leiras_cim = "A programrol";
    float lcim_w = rajz_szoveg_szelesseg(leiras_cim, leiras_cim_m);
    rajz_szoveg(leiras_cim, (szel - lcim_w) / 2.0f,
                doboz_y + 18.0f * scale,
                leiras_cim_m, 0.70f, 0.85f, 0.75f);

    // Leiras sorok – MINDEN sor KOZEPEN
    float ly = doboz_y + doboz_felso_margo;
    for (int i = 0; i < leiras_db; i++) {
        float w = rajz_szoveg_szelesseg(leiras[i], leiras_szoveg_m);
        // A doboz kozepet vesszuk, es a sor szelesseget levonva -> kozep
        rajz_szoveg(leiras[i],
                    doboz_x + (doboz_w - w) / 2.0f,
                    ly,
                    leiras_szoveg_m, 0.82f, 0.85f, 0.88f);
        ly += leiras_sor_koz;
    }

    y += doboz_mag;
    y += 20.0f * scale;

    // ---- GitHub link ----
    float gc_m = 13.0f * scale;
    const char *gh_cimke = "GitHub:";
    float gc_w = rajz_szoveg_szelesseg(gh_cimke, gc_m);
    rajz_szoveg(gh_cimke, (szel - gc_w) / 2.0f, y, gc_m,
                0.55f, 0.62f, 0.72f);
    y += gc_m + 6.0f * scale;

    float gl_m = 17.0f * scale;
    const char *gh_link = "github.com/arosdami/ChessGL";
    float gl_w = rajz_szoveg_szelesseg(gh_link, gl_m);
    rajz_szoveg(gh_link, (szel - gl_w) / 2.0f, y, gl_m,
                0.55f, 0.75f, 1.00f);

    y += link_sav;

    // ---- Sziv ----
    float sziv_meret = 36.0f * scale;
    sziv_rajzol(szel / 2.0f, y + sziv_meret * 0.6f,
                sziv_meret, 0.95f, 0.20f, 0.30f);

    // ---- Sugo ----
    const char *sugo = "Nyomj ESC-et a visszalepeshez!";
    float sm = 16.0f * scale;
    float sw = rajz_szoveg_szelesseg(sugo, sm);
    rajz_szoveg(sugo, (szel - sw) / 2.0f, mag - 25.0f,
                sm, 0.55f, 0.58f, 0.65f);
}

// ============================================================
//                       KILEPES
// ============================================================
static void kilepes_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    rajz_torol(0.07f, 0.08f, 0.11f);

    const char *kerdes = "Biztosan kilepsz?";
    float m = 44.0f;
    float w = rajz_szoveg_szelesseg(kerdes, m);
    rajz_szoveg(kerdes, (szel - w) / 2.0f, mag / 2.0f - 80.0f,
                m, 0.95f, 0.85f, 0.55f);

    const char *sugo = "I = igen        N = nem";
    float sm = 26.0f;
    float sw = rajz_szoveg_szelesseg(sugo, sm);
    rajz_szoveg(sugo, (szel - sw) / 2.0f, mag / 2.0f + 20.0f,
                sm, 0.75f, 0.78f, 0.85f);
}

// ============================================================
//                       BABU RAJZOLAS
// ============================================================
static void babu_rajzol(Babu b, float x, float y, float mezo) {
    float cx = x + mezo * 0.5f;
    float cy = y + mezo * 0.5f;
    float r  = mezo * 0.36f;
    float v  = (b.szin == FEHER) ? 1.0f : 0.08f;

    switch (b.tipus) {
        case GYALOG:
            rajz_kor(cx, cy - r*0.20f, r*0.55f, v, v, v);
            rajz_kor(cx, cy + r*0.30f, r*0.35f, v, v, v);
            break;
        case BASTYA:
            rajz_negyzet(cx - r*0.55f, cy - r*0.55f, r*1.10f, r*1.10f, v, v, v);
            rajz_negyzet(cx - r*0.70f, cy - r*0.70f, r*0.25f, r*0.25f, v, v, v);
            rajz_negyzet(cx + r*0.45f, cy - r*0.70f, r*0.25f, r*0.25f, v, v, v);
            break;
        case HUSZAR:
            rajz_kor(cx, cy - r*0.10f, r*0.55f, v, v, v);
            rajz_kor(cx - r*0.35f, cy + r*0.45f, r*0.22f, v, v, v);
            rajz_kor(cx + r*0.35f, cy + r*0.45f, r*0.22f, v, v, v);
            break;
        case FUTO:
            rajz_kor(cx, cy + r*0.10f, r*0.55f, v, v, v);
            rajz_kor(cx, cy - r*0.45f, r*0.22f, v, v, v);
            break;
        case KIRALYNO:
            rajz_kor(cx, cy - r*0.05f, r*0.60f, v, v, v);
            rajz_kor(cx - r*0.45f, cy + r*0.45f, r*0.16f, v, v, v);
            rajz_kor(cx + r*0.45f, cy + r*0.45f, r*0.16f, v, v, v);
            rajz_kor(cx, cy + r*0.55f, r*0.16f, v, v, v);
            break;
        case KIRALY:
            rajz_kor(cx, cy - r*0.05f, r*0.60f, v, v, v);
            rajz_negyzet(cx - r*0.08f, cy + r*0.45f, r*0.16f, r*0.55f, v, v, v);
            rajz_negyzet(cx - r*0.25f, cy + r*0.62f, r*0.50f, r*0.16f, v, v, v);
            break;
        default: break;
    }
}

// ============================================================
//                  PANEL ELRENDEZES + JATEK
// ============================================================
typedef struct { float x, y, w, h; } Teglalap;

static void panelek_szamol(Teglalap *bal, Teglalap *jobb_felul, Teglalap *jobb_alul) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    float margo = 20.0f;
    float koz   = 14.0f;

    float bal_w = (szel - 3 * margo - koz) * 0.78f;
    if (bal_w < 500.0f) bal_w = 500.0f;

    float jobb_w = szel - 3 * margo - koz - bal_w;
    if (jobb_w < 220.0f) jobb_w = 220.0f;

    bal->x = margo;
    bal->y = margo;
    bal->w = bal_w;
    bal->h = mag - 2 * margo;

    float jx = margo + bal_w + koz;
    float jobb_felul_h = 160.0f;
    jobb_felul->x = jx;
    jobb_felul->y = margo;
    jobb_felul->w = jobb_w;
    jobb_felul->h = jobb_felul_h;

    jobb_alul->x = jx;
    jobb_alul->y = margo + jobb_felul_h + koz;
    jobb_alul->w = jobb_w;
    jobb_alul->h = mag - 2 * margo - jobb_felul_h - koz;
}

static void tabla_pozicio(Teglalap *panel, float *tabla_x, float *tabla_y, float *mezo) {
    float belso_margo = 24.0f;
    float rendelkezes = panel->w < panel->h ? panel->w : panel->h;
    rendelkezes -= 2 * belso_margo;
    float tabla_m = rendelkezes;

    *mezo    = tabla_m / MERET;
    *tabla_x = panel->x + (panel->w - tabla_m) / 2.0f;
    *tabla_y = panel->y + (panel->h - tabla_m) / 2.0f;
}

static void jatek_rajzol(JatekAllapot *j, int kijelolt[MERET][MERET]) {
    rajz_torol(0.07f, 0.08f, 0.11f);

    Teglalap bal, jobb_felul, jobb_alul;
    panelek_szamol(&bal, &jobb_felul, &jobb_alul);

    // PIROS PANEL: sakktabla
    rajz_negyzet(bal.x, bal.y, bal.w, bal.h, 0.13f, 0.10f, 0.14f);

    float tabla_x, tabla_y, mezo;
    tabla_pozicio(&bal, &tabla_x, &tabla_y, &mezo);
    float tabla_m = mezo * MERET;

    rajz_negyzet(tabla_x - 4, tabla_y - 4, tabla_m + 8, tabla_m + 8,
                 0.20f, 0.16f, 0.10f);

    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            float x = tabla_x + o * mezo;
            float y = tabla_y + s * mezo;

            int vilagos = (s + o) % 2 == 0;
            float r0, g0, b0;
            if (vilagos) { r0 = 0.92f; g0 = 0.88f; b0 = 0.78f; }
            else         { r0 = 0.42f; g0 = 0.48f; b0 = 0.55f; }

            rajz_negyzet(x, y, mezo, mezo, r0, g0, b0);

            if (kijelolt[s][o])
                rajz_negyzet(x, y, mezo, mezo, 0.15f, 0.75f, 0.35f);

            if (j->van_kivalasztva &&
                j->kivalasztott_sor == s &&
                j->kivalasztott_oszlop == o)
                rajz_negyzet(x, y, mezo, mezo, 0.95f, 0.80f, 0.15f);

            Babu b = j->tabla[s][o];
            if (b.tipus != URES)
                babu_rajzol(b, x, y, mezo);
        }
    }

    const char *oszlopok[8] = {"a","b","c","d","e","f","g","h"};
    const char *sorok[8]    = {"8","7","6","5","4","3","2","1"};
    float betu_m = mezo * 0.24f;
    if (betu_m < 10.0f) betu_m = 10.0f;
    if (betu_m > 18.0f) betu_m = 18.0f;

    for (int o = 0; o < 8; o++) {
        float w = rajz_szoveg_szelesseg(oszlopok[o], betu_m);
        float mx = tabla_x + o * mezo + mezo / 2.0f - w / 2.0f;
        rajz_szoveg(oszlopok[o], mx, tabla_y + tabla_m + 6.0f,
                    betu_m, 0.70f, 0.72f, 0.78f);
    }
    for (int s = 0; s < 8; s++) {
        float my = tabla_y + s * mezo + mezo / 2.0f - betu_m / 2.0f;
        rajz_szoveg(sorok[s], tabla_x - betu_m - 4.0f, my,
                    betu_m, 0.70f, 0.72f, 0.78f);
    }

    // KEK PANEL: cim
    rajz_negyzet(jobb_felul.x, jobb_felul.y, jobb_felul.w, jobb_felul.h,
                 0.10f, 0.14f, 0.22f);
    rajz_negyzet(jobb_felul.x, jobb_felul.y, jobb_felul.w, 3.0f,
                 0.35f, 0.55f, 0.85f);

    float cim_m = 36.0f;
    float cim_w = rajz_szoveg_szelesseg("ChessGL", cim_m);
    float cx = jobb_felul.x + (jobb_felul.w - cim_w) / 2.0f;
    rajz_szoveg("ChessGL", cx, jobb_felul.y + 30.0f, cim_m,
                0.95f, 0.85f, 0.55f);

    float alcim_m = 16.0f;
    float alcim_w = rajz_szoveg_szelesseg("by arosdami", alcim_m);
    float acx = jobb_felul.x + (jobb_felul.w - alcim_w) / 2.0f;
    rajz_szoveg("by arosdami", acx, jobb_felul.y + 80.0f, alcim_m,
                0.55f, 0.65f, 0.80f);

    float cimke_m = 12.0f;
    float cimke_w = rajz_szoveg_szelesseg("OpenGL 3.3 Core", cimke_m);
    float ccx = jobb_felul.x + (jobb_felul.w - cimke_w) / 2.0f;
    rajz_szoveg("OpenGL 3.3 Core", ccx, jobb_felul.y + 115.0f, cimke_m,
                0.45f, 0.55f, 0.70f);

    // ZOLD PANEL: info
    rajz_negyzet(jobb_alul.x, jobb_alul.y, jobb_alul.w, jobb_alul.h,
                 0.10f, 0.16f, 0.12f);
    rajz_negyzet(jobb_alul.x, jobb_alul.y, jobb_alul.w, 3.0f,
                 0.35f, 0.80f, 0.45f);

    float px = jobb_alul.x + 15.0f;
    float py = jobb_alul.y + 24.0f;
    float sor_koz = 24.0f;

    const char *mod_szoveg = (menu_jatekmod() == MOD_GEP_ELLEN)
                             ? "Mod: Gep ellen (AI)"
                             : "Mod: 2 jatekos";
    rajz_szoveg(mod_szoveg, px, py, 13.0f, 0.55f, 0.70f, 0.60f);
    py += 22.0f;

    rajz_szoveg("Kovetkezo:", px, py, 12.0f, 0.55f, 0.70f, 0.60f);
    py += 18.0f;
    const char *ki = (j->kovetkezo == FEHER) ? "Vilagos" : "Sotet";
    float sz_r = (j->kovetkezo == FEHER) ? 0.95f : 0.35f;
    float sz_g = (j->kovetkezo == FEHER) ? 0.95f : 0.35f;
    float sz_b = (j->kovetkezo == FEHER) ? 0.95f : 0.35f;
    rajz_szoveg(ki, px, py, 22.0f, sz_r, sz_g, sz_b);
    py += 35.0f;

    rajz_szoveg("Utolso lepes:", px, py, 12.0f, 0.55f, 0.70f, 0.60f);
    py += 18.0f;
    rajz_szoveg(j->utolso_lepes, px, py, 20.0f, 0.90f, 0.95f, 0.90f);
    py += 40.0f;

    if (j->jatek_vege == 1) {
        rajz_szoveg("MATT!", px, py, 26.0f, 0.95f, 0.35f, 0.35f);
        py += 40.0f;
    } else if (j->jatek_vege == 2) {
        rajz_szoveg("PATT!", px, py, 26.0f, 0.95f, 0.80f, 0.35f);
        py += 40.0f;
    }

    if (easter_egg_aktivalva) {
        rajz_szoveg("Easter Egg :)", px, py, 14.0f, 0.85f, 0.35f, 0.85f);
        py += 30.0f;
    }

    py += 8.0f;
    rajz_szoveg("R = uj jatek",     px, py, 14.0f, 0.55f, 0.70f, 0.60f); py += sor_koz;
    rajz_szoveg("ESC = fomenu",     px, py, 14.0f, 0.55f, 0.70f, 0.60f); py += sor_koz;
    rajz_szoveg("F11 = teljes kep", px, py, 14.0f, 0.55f, 0.70f, 0.60f);
}

// ============================================================
//                  KATTINTASKEZELES
// ============================================================
static void fomenu_kattintas(void) {
    if (!eger_kattintott()) return;

    double mx = rajz_eger_x();
    double my = rajz_eger_y();

    for (int i = 0; i < FOMENU_GOMB_DB; i++) {
        Gomb *g = &fomenu_gombok[i];
        if (mx >= g->x && mx <= g->x + g->w &&
            my >= g->y && my <= g->y + g->h) {
            kivalasztott_gomb = i;
            return;
        }
    }
}

static void jatek_kattintas(JatekAllapot *j) {
    if (menu_jatekmod() == MOD_GEP_ELLEN && j->kovetkezo == FEKETE) return;
    if (j->jatek_vege) return;

    if (!eger_kattintott()) return;

    Teglalap bal, jf, ja;
    panelek_szamol(&bal, &jf, &ja);
    float tabla_x, tabla_y, mezo;
    tabla_pozicio(&bal, &tabla_x, &tabla_y, &mezo);

    double mx = rajz_eger_x();
    double my = rajz_eger_y();

    int o = (int)((mx - tabla_x) / mezo);
    int s = (int)((my - tabla_y) / mezo);

    if (s < 0 || s >= 8 || o < 0 || o >= 8) return;

    if (!j->van_kivalasztva) {
        Babu b = j->tabla[s][o];
        if (b.tipus != URES && b.szin == j->kovetkezo) {
            j->van_kivalasztva     = 1;
            j->kivalasztott_sor    = s;
            j->kivalasztott_oszlop = o;
        }
    } else {
        if (s == j->kivalasztott_sor && o == j->kivalasztott_oszlop) {
            j->van_kivalasztva = 0;
            return;
        }
        if (lepes_vegrehajtas(j, j->kivalasztott_sor, j->kivalasztott_oszlop,
                              s, o)) {
            if (!van_legalis_lepes(j, j->kovetkezo)) {
                j->jatek_vege = 1;
            }
            return;
        }
        Babu b = j->tabla[s][o];
        if (b.tipus != URES && b.szin == j->kovetkezo) {
            j->kivalasztott_sor    = s;
            j->kivalasztott_oszlop = o;
        }
    }
}

// ============================================================
//                  CALLBACKEK
// ============================================================
static void ablak_meret_callback(GLFWwindow *w, int szel, int mag) {
    (void)w;
    rajz_ablak_meret(szel, mag);
}

static void billentyu_callback(GLFWwindow *w, int key, int sc, int act, int mod) {
    (void)sc; (void)mod;
    if (act != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        Kepernyo k = menu_aktualis();
        if (k == KEPERNYO_JATEK || k == KEPERNYO_NEVJEGY) {
            menu_valtas(KEPERNYO_FOMENU);
        } else if (k == KEPERNYO_FOMENU) {
            glfwSetWindowShouldClose(w, 1);
        }
    }
    if (key == GLFW_KEY_F11) {
        rajz_teljes_kepernyo(!rajz_teljes_kepernyos());
    }
    if (key == GLFW_KEY_E) {
        printf("EASTER EGG: Aros Damjan titkos uzenete! A kave elfogyott, de a kod lefordul!\n");
        easter_egg_aktivalva = !easter_egg_aktivalva;
    }
}

// ============================================================
//                       MAIN
// ============================================================
int main(void) {
    if (!rajz_inicializalas()) {
        printf("Hiba: nem sikerult inicializalni az OpenGL ablakot.\n");
        return 1;
    }

    GLFWwindow *w = glfwGetCurrentContext();
    glfwSetWindowTitle(w, "ChessGL by arosdami");
    glfwSetWindowSizeCallback(w, ablak_meret_callback);
    glfwSetKeyCallback(w, billentyu_callback);

    rajz_teljes_kepernyo(1);

    {
        int akt_w, akt_h;
        glfwGetFramebufferSize(w, &akt_w, &akt_h);
        rajz_ablak_meret(akt_w, akt_h);
    }

    menu_inicializalas();
    JatekAllapot jatek;
    tabla_inicializalas(&jatek);
    int kijelolt[MERET][MERET];
    memset(kijelolt, 0, sizeof(kijelolt));

    printf("ChessGL by arosdami elindult.\n");
    printf("F11 = teljes kepernyo ki/be\n");
    printf("E   = easter egg\n");

    while (!rajz_ablak_bezarva()) {
        Kepernyo k = menu_aktualis();

        if (k == KEPERNYO_FOMENU) {
            fomenu_rajzol();
            fomenu_kattintas();
            if (kivalasztott_gomb == 0) {
                tabla_inicializalas(&jatek);
                memset(kijelolt, 0, sizeof(kijelolt));
                menu_jatekmod_beallit(MOD_KET_JATEKOS);
                menu_valtas(KEPERNYO_JATEK);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 1) {
                tabla_inicializalas(&jatek);
                memset(kijelolt, 0, sizeof(kijelolt));
                menu_jatekmod_beallit(MOD_GEP_ELLEN);
                menu_valtas(KEPERNYO_JATEK);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 2) {
                menu_valtas(KEPERNYO_NEVJEGY);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 3) {
                menu_valtas(KEPERNYO_KILEPES);
                kivalasztott_gomb = -1;
            }
        } else if (k == KEPERNYO_JATEK) {
            memset(kijelolt, 0, sizeof(kijelolt));
            if (jatek.van_kivalasztva) {
                lepesek_listazasa(&jatek, jatek.kivalasztott_sor,
                                  jatek.kivalasztott_oszlop, kijelolt);
            }
            jatek_rajzol(&jatek, kijelolt);
            jatek_kattintas(&jatek);

            if (menu_jatekmod() == MOD_GEP_ELLEN &&
                jatek.kovetkezo == FEKETE &&
                !jatek.jatek_vege) {

                int sr, so, cr, co;
                if (ai_lepest_valaszt(&jatek, FEKETE, &sr, &so, &cr, &co)) {
                    lepes_vegrehajtas(&jatek, sr, so, cr, co);
                    if (!van_legalis_lepes(&jatek, jatek.kovetkezo)) {
                        jatek.jatek_vege = 1;
                    }
                }
            }
        } else if (k == KEPERNYO_NEVJEGY) {
            nevjegy_rajzol();
        } else if (k == KEPERNYO_KILEPES) {
            kilepes_rajzol();
            if (glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS) {
                glfwSetWindowShouldClose(w, 1);
            }
            if (glfwGetKey(w, GLFW_KEY_N) == GLFW_PRESS) {
                menu_valtas(KEPERNYO_FOMENU);
            }
        }

        rajz_eszkozok_cserel();
    }

    rajz_bezaras();
    return 0;
}