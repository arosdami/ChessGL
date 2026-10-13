#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "rajz.h"
#include "tabla.h"
#include "menu.h"
#include "ai.h"
#include "babu.h"

// ============================================================
//                    GLOBALIS VALTOZOK
// ============================================================
static unsigned int creator_textura = 0;

// ============================================================
//                    GOMBOK A FOMENUBEN
// ============================================================
typedef struct {
    const char *felirat;
    float x, y, w, h;
} Gomb;

static Gomb fomenu_gombok[] = {
    {"Folytatas", 0, 0, 0, 0},
    {"Uj jatek",  0, 0, 0, 0},
    {"AiChess",   0, 0, 0, 0},
    {"Nevjegy",   0, 0, 0, 0},
    {"Kilepes",   0, 0, 0, 0},
};
static const int FOMENU_GOMB_DB = 5;

static int eger_elozo = 0;
static int kivalasztott_gomb = -1;
static int easter_egg_aktivalva = 0;
static int van_folytathato_jatek = 0;
static int uj_jatek_keresve = 0;

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
//                       FOMENU
// ============================================================
static void fomenu_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    rajz_torol(0.07f, 0.08f, 0.11f);

    float ui = mag < 760 ? mag / 760.0f : 1.0f;
    if (ui < 0.72f) ui = 0.72f;
    float cim_meret   = 70.0f * ui;
    float alcim_meret = 26.0f * ui;
    float gomb_w      = 360.0f * ui;
    float gomb_h      = 55.0f * ui;
    float gomb_koz    = 12.0f * ui;

    float cim_sav     = cim_meret + 12.0f;
    float alcim_sav   = alcim_meret + 50.0f;
    float gombok_sav  = FOMENU_GOMB_DB * gomb_h + (FOMENU_GOMB_DB - 1) * gomb_koz;
    float blokk_mag   = cim_sav + alcim_sav + gombok_sav;

    float blokk_y = (mag - blokk_mag) / 2.0f;
    if (blokk_y < 18.0f) blokk_y = 18.0f;

    const char *cim = "ChessGL";
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

        int aktiv = i != 0 || van_folytathato_jatek;
        if (folotte && aktiv)
            rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, gomb_h,
                         0.20f, 0.28f, 0.42f);
        else
            rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, gomb_h,
                         0.14f, 0.16f, 0.22f);

        rajz_negyzet(x, fomenu_gombok[i].y, gomb_w, 2.0f,
                     folotte && aktiv ? 0.75f : 0.35f,
                     folotte && aktiv ? 0.65f : 0.38f,
                     folotte && aktiv ? 0.35f : 0.50f);

        float t_meret = 22.0f;
        float t_szel  = rajz_szoveg_szelesseg(fomenu_gombok[i].felirat, t_meret);
        rajz_szoveg(fomenu_gombok[i].felirat,
                    x + (gomb_w - t_szel) / 2.0f,
                    fomenu_gombok[i].y + (gomb_h - t_meret) / 2.0f,
                    t_meret,
                    !aktiv ? 0.42f : (folotte ? 1.0f : 0.85f),
                    !aktiv ? 0.45f : (folotte ? 1.0f : 0.85f),
                    !aktiv ? 0.50f : (folotte ? 1.0f : 0.85f));
    }

    const char *lablec = "F11 = teljes kepernyo     ESC = vissza";
    float l_meret = 14.0f;
    float l_szel  = rajz_szoveg_szelesseg(lablec, l_meret);
    rajz_szoveg(lablec, (szel - l_szel) / 2.0f, mag - 25.0f,
                l_meret, 0.45f, 0.48f, 0.55f);
}

static void ai_nehezseg_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag = rajz_ablak_mag();
    const char *feliratok[] = {"Konnyu", "Normal", "Eros"};
    float cim_m = 42.0f;
    float gomb_w = 330.0f;
    float gomb_h = 58.0f;
    float koz = 16.0f;
    float blokk = cim_m + 55.0f + 3 * gomb_h + 2 * koz;
    float y = (mag - blokk) / 2.0f;

    rajz_torol(0.07f, 0.08f, 0.11f);
    float cw = rajz_szoveg_szelesseg("AI EROSSEG", cim_m);
    rajz_szoveg("AI EROSSEG", (szel - cw) / 2.0f, y, cim_m,
                0.95f, 0.85f, 0.55f);
    y += cim_m + 55.0f;

    for (int i = 0; i < 3; i++) {
        float x = (szel - gomb_w) / 2.0f;
        int folotte = eger_benne(x, y, gomb_w, gomb_h);
        rajz_negyzet(x, y, gomb_w, gomb_h,
                     folotte ? 0.20f : 0.14f,
                     folotte ? 0.28f : 0.16f,
                     folotte ? 0.42f : 0.22f);
        float tm = 22.0f;
        float tw = rajz_szoveg_szelesseg(feliratok[i], tm);
        rajz_szoveg(feliratok[i], x + (gomb_w - tw) / 2.0f,
                    y + (gomb_h - tm) / 2.0f, tm,
                    0.9f, 0.92f, 0.98f);
        y += gomb_h + koz;
    }

    const char *sugo = "ESC = vissza a fomenube";
    float sm = 15.0f;
    float sw = rajz_szoveg_szelesseg(sugo, sm);
    rajz_szoveg(sugo, (szel - sw) / 2.0f, mag - 30.0f,
                sm, 0.55f, 0.60f, 0.70f);
}

static int ai_nehezseg_kattintas(void) {
    if (!eger_kattintott()) return 0;
    int szel = rajz_ablak_szel();
    int mag = rajz_ablak_mag();
    float gomb_w = 330.0f, gomb_h = 58.0f, koz = 16.0f;
    float y = (mag - (42.0f + 55.0f + 3 * gomb_h + 2 * koz)) / 2.0f;
    y += 42.0f + 55.0f;
    for (int i = 0; i < 3; i++) {
        float x = (szel - gomb_w) / 2.0f;
        if (eger_benne(x, y, gomb_w, gomb_h)) {
            ai_nehezseg_beallit(i + 1);
            return 1;
        }
        y += gomb_h + koz;
    }
    return 0;
}

// ============================================================
//                       NEVJEGY
// ============================================================
static void nevjegy_rajzol(void) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    rajz_torol(0.07f, 0.08f, 0.11f);

    float scale = 1.0f;
    if (mag < 800.0f) scale = mag / 800.0f;
    if (scale < 0.55f) scale = 0.55f;

    // ---- Meretek ----
    float kep_sugar = 80.0f * scale;
    float kep_sav   = 2 * kep_sugar + 30.0f * scale;

    float cim_meret = 40.0f * scale;
    float cim_sav   = cim_meret + 30.0f * scale;

    float cimke_m = 14.0f * scale;
    float ertek_m = 24.0f * scale;
    float par_mag = cimke_m + 6.0f * scale + ertek_m + 24.0f * scale;
    float fejlec_sav = 3 * par_mag;

    float link_sav = 60.0f * scale;
    float sziv_sav = 60.0f * scale;

    // ---- Blokk magassag ----
    float blokk_mag = kep_sav + cim_sav + fejlec_sav + link_sav + sziv_sav;

    float blokk_y = (mag - blokk_mag) / 2.0f;
    if (blokk_y < 15.0f) blokk_y = 15.0f;

    float y = blokk_y;

    // ---- KEP (kor alakban) ----
    if (creator_textura != 0) {
        rajz_textura_kor(creator_textura, szel / 2.0f,
                         y + kep_sugar, kep_sugar);
    } else {
        rajz_kor(szel / 2.0f, y + kep_sugar, kep_sugar,
                 0.35f, 0.65f, 0.80f);
    }

    y += kep_sav;

    // ---- CIM ----
    const char *cim = "NEVJEGY";
    float cim_szel = rajz_szoveg_szelesseg(cim, cim_meret);
    rajz_szoveg(cim, (szel - cim_szel) / 2.0f, y,
                cim_meret, 0.95f, 0.85f, 0.55f);

    y += cim_sav;

    // ---- FEJLEC ----
    const char *cimkek[] = { "Keszitette:", "Tantargy:", "Oktato:" };
    const char *ertekek[] = {
        "Aros Damjan",
        "3D modellezes es jatekfejlesztes",
        "Dr. Mileff Peter"
    };

    for (int i = 0; i < 3; i++) {
        float cw = rajz_szoveg_szelesseg(cimkek[i], cimke_m);
        rajz_szoveg(cimkek[i], (szel - cw) / 2.0f, y,
                    cimke_m, 0.55f, 0.62f, 0.72f);
        y += cimke_m + 6.0f * scale;

        float ew = rajz_szoveg_szelesseg(ertekek[i], ertek_m);
        rajz_szoveg(ertekek[i], (szel - ew) / 2.0f, y,
                    ertek_m, 0.95f, 0.92f, 0.78f);
        y += ertek_m + 24.0f * scale;
    }

    // ---- GITHUB ----
    float gc_m = 14.0f * scale;
    const char *gh_c = "GitHub:";
    float gc_w = rajz_szoveg_szelesseg(gh_c, gc_m);
    rajz_szoveg(gh_c, (szel - gc_w) / 2.0f, y, gc_m,
                0.55f, 0.62f, 0.72f);
    y += gc_m + 8.0f * scale;

    float gl_m = 18.0f * scale;
    const char *gh_l = "github.com/arosdami/ChessGL";
    float gl_w = rajz_szoveg_szelesseg(gh_l, gl_m);
    rajz_szoveg(gh_l, (szel - gl_w) / 2.0f, y, gl_m,
                0.55f, 0.75f, 1.00f);

    y += link_sav;

    // ---- SZIV ----
    sziv_rajzol(szel / 2.0f, y + 30.0f * scale * 0.6f,
                36.0f * scale, 0.95f, 0.20f, 0.30f);

    // ---- SUGO ----
    const char *sugo = "Nyomj ESC-et a visszalepeshez!";
    float sm = 15.0f * scale;
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
//                  PANEL ELRENDEZES
// ============================================================
typedef struct { float x, y, w, h; } Teglalap;

static void panelek_szamol(Teglalap *bal, Teglalap *jf, Teglalap *ja) {
    int szel = rajz_ablak_szel();
    int mag  = rajz_ablak_mag();

    float margo = szel < 1100 ? 14.0f : 24.0f;
    float koz   = szel < 1100 ? 12.0f : 18.0f;
    float jobb_w = szel * 0.27f;
    if (jobb_w < 250.0f) jobb_w = 250.0f;
    if (jobb_w > 380.0f) jobb_w = 380.0f;
    float bal_w = szel - 2 * margo - koz - jobb_w;

    float also_sav = 40.0f;
    bal->x = margo; bal->y = margo;
    bal->w = bal_w; bal->h = mag - 2 * margo - also_sav;

    float jx = margo + bal_w + koz;
    float jfh = 160.0f;
    jf->x = jx; jf->y = margo;
    jf->w = jobb_w; jf->h = jfh;

    ja->x = jx;
    ja->y = margo + jfh + koz;
    ja->w = jobb_w;
    ja->h = mag - 2 * margo - jfh - koz - also_sav;
}

static void tabla_pozicio(Teglalap *panel, float *tx, float *ty, float *mezo) {
    float belso = 18.0f;
    float r = panel->w < panel->h ? panel->w : panel->h;
    r -= 2 * belso;
    *mezo = r / MERET;
    *tx = panel->x + (panel->w - r) / 2.0f;
    *ty = panel->y + (panel->h - r) / 2.0f;
}

static void jatek_rajzol(JatekAllapot *j, int kijelolt[MERET][MERET]) {
    rajz_torol(0.07f, 0.08f, 0.11f);

    Teglalap bal, jf, ja;
    panelek_szamol(&bal, &jf, &ja);

    rajz_negyzet(bal.x, bal.y, bal.w, bal.h, 0.13f, 0.10f, 0.14f);

    float tx, ty, mezo;
    tabla_pozicio(&bal, &tx, &ty, &mezo);
    float tm = mezo * MERET;

    rajz_negyzet(tx - 4, ty - 4, tm + 8, tm + 8, 0.20f, 0.16f, 0.10f);

    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            float x = tx + o * mezo;
            float y = ty + s * mezo;
            int vilagos = (s + o) % 2 == 0;
            float r0 = vilagos ? 0.92f : 0.42f;
            float g0 = vilagos ? 0.88f : 0.48f;
            float b0 = vilagos ? 0.78f : 0.55f;
            rajz_negyzet(x, y, mezo, mezo, r0, g0, b0);

            if (kijelolt[s][o])
                rajz_negyzet(x, y, mezo, mezo, 0.15f, 0.75f, 0.35f);

            if (j->van_kivalasztva &&
                j->kivalasztott_sor == s && j->kivalasztott_oszlop == o)
                rajz_negyzet(x, y, mezo, mezo, 0.95f, 0.80f, 0.15f);

            Babu b = j->tabla[s][o];
            if (b.tipus != URES) babu_rajzol(b, x, y, mezo);
        }
    }

    const char *oszlopok[8] = {"a","b","c","d","e","f","g","h"};
    const char *sorok[8]    = {"8","7","6","5","4","3","2","1"};
    float betu_m = mezo * 0.24f;
    if (betu_m < 10.0f) betu_m = 10.0f;
    if (betu_m > 18.0f) betu_m = 18.0f;

    for (int o = 0; o < 8; o++) {
        float w = rajz_szoveg_szelesseg(oszlopok[o], betu_m);
        rajz_szoveg(oszlopok[o], tx + o * mezo + mezo/2.0f - w/2.0f,
                    ty + tm + 6.0f, betu_m, 0.70f, 0.72f, 0.78f);
    }
    for (int s = 0; s < 8; s++) {
        rajz_szoveg(sorok[s], tx - betu_m - 4.0f,
                    ty + s * mezo + mezo/2.0f - betu_m/2.0f,
                    betu_m, 0.70f, 0.72f, 0.78f);
    }

    // Kek panel
    rajz_negyzet(jf.x, jf.y, jf.w, jf.h, 0.10f, 0.14f, 0.22f);
    rajz_negyzet(jf.x, jf.y, jf.w, 3.0f, 0.35f, 0.55f, 0.85f);

    float cim_m = 36.0f;
    float cw = rajz_szoveg_szelesseg("ChessGL", cim_m);
    rajz_szoveg("ChessGL", jf.x + (jf.w - cw)/2.0f, jf.y + 30.0f,
                cim_m, 0.95f, 0.85f, 0.55f);

    float am = 16.0f;
    float aw = rajz_szoveg_szelesseg("by arosdami", am);
    rajz_szoveg("by arosdami", jf.x + (jf.w - aw)/2.0f, jf.y + 80.0f,
                am, 0.55f, 0.65f, 0.80f);

    float km = 12.0f;
    float kw = rajz_szoveg_szelesseg("OpenGL 3.3 Core", km);
    rajz_szoveg("OpenGL 3.3 Core", jf.x + (jf.w - kw)/2.0f, jf.y + 115.0f,
                km, 0.45f, 0.55f, 0.70f);

    // Zold panel
    rajz_negyzet(ja.x, ja.y, ja.w, ja.h, 0.10f, 0.16f, 0.12f);
    rajz_negyzet(ja.x, ja.y, ja.w, 3.0f, 0.35f, 0.80f, 0.45f);

    float px = ja.x + 15.0f;
    float py = ja.y + 24.0f;

    const char *mod = (menu_jatekmod() == MOD_GEP_ELLEN)
                      ? "Mod: Gep ellen (AI)" : "Mod: 2 jatekos";
    rajz_szoveg(mod, px, py, 13.0f, 0.55f, 0.70f, 0.60f);
    py += 22.0f;
    if (menu_jatekmod() == MOD_GEP_ELLEN) {
        const char *ero = ai_nehezseg() == 1 ? "AI: konnyu" :
                          (ai_nehezseg() == 2 ? "AI: normal" : "AI: eros");
        rajz_szoveg(ero, px, py, 13.0f, 0.55f, 0.70f, 0.60f);
        py += 22.0f;
    }

    rajz_szoveg("Kovetkezo:", px, py, 12.0f, 0.55f, 0.70f, 0.60f);
    py += 18.0f;
    const char *ki = (j->kovetkezo == FEHER) ? "Vilagos" : "Sotet";
    float sz = (j->kovetkezo == FEHER) ? 0.95f : 0.35f;
    rajz_szoveg(ki, px, py, 22.0f, sz, sz, sz);
    py += 35.0f;

    rajz_szoveg("Utolso lepes:", px, py, 12.0f, 0.55f, 0.70f, 0.60f);
    py += 18.0f;
    rajz_szoveg(j->utolso_lepes, px, py, 20.0f, 0.90f, 0.95f, 0.90f);
    py += 40.0f;

    if (j->jatek_vege == 1) {
        rajz_szoveg("MATT!", px, py, 26.0f, 0.95f, 0.35f, 0.35f);
        py += 40.0f;
    }

    if (easter_egg_aktivalva) {
        rajz_szoveg("Easter Egg :)", px, py, 14.0f, 0.85f, 0.35f, 0.85f);
        py += 30.0f;
    }

    float also_y = rajz_ablak_mag() - 34.0f;
    rajz_negyzet(0.0f, also_y - 6.0f, (float)rajz_ablak_szel(), 40.0f,
                 0.06f, 0.07f, 0.11f);
    const char *vezerles = "R  UJ JATEK        ESC  FOMENU        F11  TELJES KEPE";
    float vm = 14.0f;
    float vw = rajz_szoveg_szelesseg(vezerles, vm);
    rajz_szoveg(vezerles, (rajz_ablak_szel() - vw) / 2.0f, also_y,
                vm, 0.70f, 0.74f, 0.82f);
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
            if (i == 0 && !van_folytathato_jatek) return;
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
    float tx, ty, mezo;
    tabla_pozicio(&bal, &tx, &ty, &mezo);

    double mx = rajz_eger_x();
    double my = rajz_eger_y();

    int o = (int)((mx - tx) / mezo);
    int s = (int)((my - ty) / mezo);

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
        } else if (k == KEPERNYO_AI_NEHEZSEG) {
            menu_valtas(KEPERNYO_FOMENU);
        } else if (k == KEPERNYO_FOMENU) {
            glfwSetWindowShouldClose(w, 1);
        }
    }
    if (key == GLFW_KEY_F11) {
        rajz_teljes_kepernyo(!rajz_teljes_kepernyos());
    }
    if (key == GLFW_KEY_R && menu_aktualis() == KEPERNYO_JATEK) {
        uj_jatek_keresve = 1;
    }
    if (key == GLFW_KEY_E) {
        printf("EASTER EGG!\n");
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

    printf("--- Texturak betoltese ---\n");
    unsigned int babuk_tex = rajz_textura_betolt("resources/babuk.png");
    babu_textura_beallit(babuk_tex);

    creator_textura = rajz_textura_betolt("resources/creator.png");
    printf("creator_textura = %u\n", creator_textura);
    printf("--------------------------\n");

    JatekAllapot jatek;
    tabla_inicializalas(&jatek);
    int kijelolt[MERET][MERET];
    memset(kijelolt, 0, sizeof(kijelolt));

    printf("ChessGL by arosdami elindult.\n");

    while (!rajz_ablak_bezarva()) {
        Kepernyo k = menu_aktualis();

        if (k == KEPERNYO_FOMENU) {
            fomenu_rajzol();
            fomenu_kattintas();
            if (kivalasztott_gomb == 0 && van_folytathato_jatek) {
                menu_valtas(KEPERNYO_JATEK);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 1) {
                tabla_inicializalas(&jatek);
                memset(kijelolt, 0, sizeof(kijelolt));
                menu_jatekmod_beallit(MOD_KET_JATEKOS);
                van_folytathato_jatek = 1;
                menu_valtas(KEPERNYO_JATEK);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 2) {
                menu_valtas(KEPERNYO_AI_NEHEZSEG);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 3) {
                menu_valtas(KEPERNYO_NEVJEGY);
                kivalasztott_gomb = -1;
            } else if (kivalasztott_gomb == 4) {
                menu_valtas(KEPERNYO_KILEPES);
                kivalasztott_gomb = -1;
            }
        } else if (k == KEPERNYO_AI_NEHEZSEG) {
            ai_nehezseg_rajzol();
            if (ai_nehezseg_kattintas()) {
                tabla_inicializalas(&jatek);
                memset(kijelolt, 0, sizeof(kijelolt));
                menu_jatekmod_beallit(MOD_GEP_ELLEN);
                van_folytathato_jatek = 1;
                menu_valtas(KEPERNYO_JATEK);
            }
        } else if (k == KEPERNYO_JATEK) {
            if (uj_jatek_keresve) {
                tabla_inicializalas(&jatek);
                memset(kijelolt, 0, sizeof(kijelolt));
                uj_jatek_keresve = 0;
            }
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