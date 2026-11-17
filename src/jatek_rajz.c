#include "ai.h"
#include "babu.h"
#include "jatek_rajz.h"
#include "menu.h"
#include "rajz.h"
#include "ui_konstansok.h"

static void szoveg_kozepre(const char *szoveg, float y, float meret,
                           float r, float g, float b) {
    float szel = rajz_ablak_szel();
    float szoveg_szel = rajz_szoveg_szelesseg(szoveg, meret);
    rajz_szoveg(szoveg, (szel - szoveg_szel) * KOZEPRE_IGAZIT,
                y, meret, r, g, b);
}

void jatek_panelek_szamol(Teglalap *bal, Teglalap *jf, Teglalap *ja) {
    int szel = rajz_ablak_szel();
    int mag = rajz_ablak_mag();

    float margo = szel < PANEL_TORZSPONT ? PANEL_MARGO_KICSI : PANEL_MARGO_NAGY;
    float koz = szel < PANEL_TORZSPONT ? PANEL_KOZ_KICSI : PANEL_KOZ_NAGY;
    float jobb_w = szel * JOBB_PANEL_ARANY;
    if (jobb_w < PANEL_JOBB_MIN) jobb_w = PANEL_JOBB_MIN;
    if (jobb_w > PANEL_JOBB_MAX) jobb_w = PANEL_JOBB_MAX;

    float bal_w = szel - MARGOK_SZORZO * margo - koz - jobb_w;
    float also_sav = PANEL_ALSO_SAV;

    bal->x = margo;
    bal->y = margo;
    bal->w = bal_w;
    bal->h = mag - MARGOK_SZORZO * margo - also_sav;

    float jx = margo + bal_w + koz;
    float jfh = PANEL_FEJLEC_MAG;
    jf->x = jx;
    jf->y = margo;
    jf->w = jobb_w;
    jf->h = jfh;

    ja->x = jx;
    ja->y = margo + jfh + koz;
    ja->w = jobb_w;
    ja->h = mag - MARGOK_SZORZO * margo - jfh - koz - also_sav;
}

void jatek_tabla_pozicio(const Teglalap *panel, float *tx, float *ty,
                         float *mezo) {
    float belso = TABLA_BELSO_MARGO;
    float r = panel->w < panel->h ? panel->w : panel->h;
    r -= MARGOK_SZORZO * belso;
    *mezo = r / MERET;
    *tx = panel->x + (panel->w - r) * KOZEPRE_IGAZIT;
    *ty = panel->y + (panel->h - r) * KOZEPRE_IGAZIT;
}

void jatek_rajzol(JatekAllapot *j, int kijelolt[MERET][MERET],
                  int easter_egg_aktivalva) {
    rajz_torol(SZIN_HATT.r, SZIN_HATT.g, SZIN_HATT.b);

    Teglalap bal, jf, ja;
    jatek_panelek_szamol(&bal, &jf, &ja);

    rajz_negyzet(bal.x, bal.y, bal.w, bal.h,
                 SZIN_PANEL_HATT.r, SZIN_PANEL_HATT.g, SZIN_PANEL_HATT.b);

    float tx, ty, mezo;
    jatek_tabla_pozicio(&bal, &tx, &ty, &mezo);
    float tm = mezo * MERET;

    rajz_negyzet(tx - UI_TABLA_KERET, ty - UI_TABLA_KERET,
                 tm + UI_TABLA_KERET_KULSO, tm + UI_TABLA_KERET_KULSO,
                 SZIN_TABLA_KERET.r, SZIN_TABLA_KERET.g, SZIN_TABLA_KERET.b);

    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            float x = tx + o * mezo;
            float y = ty + s * mezo;
            int vilagos = (s + o) % 2 == 0;

            if (vilagos) {
                rajz_negyzet(x, y, mezo, mezo,
                             SZIN_TABLA_VILAGOS.r, SZIN_TABLA_VILAGOS.g,
                             SZIN_TABLA_VILAGOS.b);
            } else {
                rajz_negyzet(x, y, mezo, mezo,
                             SZIN_TABLA_SOTET.r, SZIN_TABLA_SOTET.g,
                             SZIN_TABLA_SOTET.b);
            }

            if (kijelolt[s][o]) {
                rajz_negyzet(x, y, mezo, mezo,
                             SZIN_MEZO_KIJELOLT.r, SZIN_MEZO_KIJELOLT.g,
                             SZIN_MEZO_KIJELOLT.b);
            }

            if (j->van_kivalasztva &&
                j->kivalasztott_sor == s &&
                j->kivalasztott_oszlop == o) {
                rajz_negyzet(x, y, mezo, mezo,
                             SZIN_MEZO_VALASZT.r, SZIN_MEZO_VALASZT.g,
                             SZIN_MEZO_VALASZT.b);
            }

            Babu b = j->tabla[s][o];
            if (b.tipus != URES) babu_rajzol(b, x, y, mezo);
        }
    }

    const char *oszlopok[MERET] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    const char *sorok[MERET] = {"8", "7", "6", "5", "4", "3", "2", "1"};
    float betu_m = mezo * UI_TABLA_BETU_ARANY;
    if (betu_m < UI_TABLA_BETU_MIN) betu_m = UI_TABLA_BETU_MIN;
    if (betu_m > UI_TABLA_BETU_MAX) betu_m = UI_TABLA_BETU_MAX;

    for (int o = 0; o < MERET; o++) {
        float w = rajz_szoveg_szelesseg(oszlopok[o], betu_m);
        rajz_szoveg(oszlopok[o],
                    tx + o * mezo + mezo * KOZEPRE_IGAZIT -
                        w * KOZEPRE_IGAZIT,
                    ty + tm + TABLA_ALSO_KOZ, betu_m,
                    SZIN_ALCIM.r, SZIN_ALCIM.g, SZIN_ALCIM.b);
    }
    for (int s = 0; s < MERET; s++) {
        rajz_szoveg(sorok[s], tx - betu_m - UI_TABLA_KERET,
                    ty + s * mezo + mezo * KOZEPRE_IGAZIT -
                        betu_m * KOZEPRE_IGAZIT,
                    betu_m, SZIN_ALCIM.r, SZIN_ALCIM.g, SZIN_ALCIM.b);
    }

    rajz_negyzet(jf.x, jf.y, jf.w, jf.h,
                 SZIN_GOMB_ALAP.r, SZIN_GOMB_ALAP.g, SZIN_GOMB_ALAP.b);
    rajz_negyzet(jf.x, jf.y, jf.w, UI_ALSO_KERET,
                 SZIN_GOMB_KERET_I.r, SZIN_GOMB_KERET_I.g,
                 SZIN_GOMB_KERET_I.b);

    float cw = rajz_szoveg_szelesseg("ChessGL", PANEL_CIM_MERET);
    rajz_szoveg("ChessGL", jf.x + (jf.w - cw) * KOZEPRE_IGAZIT,
                jf.y + PANEL_CIM_FELSO, PANEL_CIM_MERET,
                SZIN_CIM.r, SZIN_CIM.g, SZIN_CIM.b);
    float aw = rajz_szoveg_szelesseg("by arosdami", UI_SZOVEG_LINK);
    rajz_szoveg("by arosdami", jf.x + (jf.w - aw) * KOZEPRE_IGAZIT,
                jf.y + PANEL_ALCIM_FELSO, UI_SZOVEG_LINK,
                SZIN_NEVJEGY_KOR.r, SZIN_NEVJEGY_KOR.g,
                SZIN_NEVJEGY_KOR.b);
    float kw = rajz_szoveg_szelesseg("OpenGL 3.3 Core", UI_SZOVEG_KICSI);
    rajz_szoveg("OpenGL 3.3 Core", jf.x + (jf.w - kw) * KOZEPRE_IGAZIT,
                jf.y + PANEL_VERZIO_FELSO, UI_SZOVEG_KICSI,
                SZIN_ALCIM.r, SZIN_ALCIM.g, SZIN_ALCIM.b);

    rajz_negyzet(ja.x, ja.y, ja.w, ja.h,
                 SZIN_PANEL_ALSO.r, SZIN_PANEL_ALSO.g, SZIN_PANEL_ALSO.b);
    rajz_negyzet(ja.x, ja.y, ja.w, UI_ALSO_KERET,
                 SZIN_GOMB_KERET_A.r, SZIN_GOMB_KERET_A.g,
                 SZIN_GOMB_KERET_A.b);

    float px = ja.x + UI_PANEL_BAL_KOZ;
    float py = ja.y + UI_PANEL_FELSO_KOZ;
    const char *mod = (menu_jatekmod() == MOD_GEP_ELLEN)
        ? "Mod: Gep ellen (AI)" : "Mod: 2 jatekos";
    rajz_szoveg(mod, px, py, UI_SZOVEG_PANEL,
                SZIN_PANEL_SZOVEG.r, SZIN_PANEL_SZOVEG.g,
                SZIN_PANEL_SZOVEG.b);
    py += UI_PANEL_SOR_KOZ;

    if (menu_jatekmod() == MOD_GEP_ELLEN) {
        const char *ero = ai_nehezseg() == AI_MIN_MELYSEG
            ? "AI: konnyu"
            : (ai_nehezseg() == AI_NORMAL_MELYSEG ? "AI: normal" : "AI: eros");
        rajz_szoveg(ero, px, py, UI_SZOVEG_PANEL,
                    SZIN_PANEL_SZOVEG.r, SZIN_PANEL_SZOVEG.g,
                    SZIN_PANEL_SZOVEG.b);
        py += UI_PANEL_SOR_KOZ;
    }

    rajz_szoveg("Kovetkezo:", px, py, UI_SZOVEG_KICSI,
                SZIN_PANEL_SZOVEG.r, SZIN_PANEL_SZOVEG.g,
                SZIN_PANEL_SZOVEG.b);
    py += UI_PANEL_CIMKE_KOZ;
    const char *ki = (j->kovetkezo == FEHER) ? "Vilagos" : "Sotet";
    SzinRGB jatekos_szin = (j->kovetkezo == FEHER)
        ? SZIN_JATEKOS_VILAGOS : SZIN_JATEKOS_SOTET;
    rajz_szoveg(ki, px, py, UI_SZOVEG_JATEK,
                jatekos_szin.r, jatekos_szin.g, jatekos_szin.b);
    py += UI_PANEL_ERTEK_KOZ;

    rajz_szoveg("Utolso lepes:", px, py, UI_SZOVEG_KICSI,
                SZIN_PANEL_SZOVEG.r, SZIN_PANEL_SZOVEG.g,
                SZIN_PANEL_SZOVEG.b);
    py += UI_PANEL_CIMKE_KOZ;
    rajz_szoveg(j->utolso_lepes, px, py, UI_SZOVEG_NAGY,
                SZIN_SZOVEG_ZOLDES.r, SZIN_SZOVEG_ZOLDES.g,
                SZIN_SZOVEG_ZOLDES.b);
    py += UI_PANEL_LEPES_KOZ;

    if (j->jatek_vege == JATEK_MATT) {
        rajz_szoveg("MATT!", px, py, UI_SZOVEG_CIM,
                    SZIN_MATT.r, SZIN_MATT.g, SZIN_MATT.b);
        py += UI_PANEL_LEPES_KOZ;
    }

    if (easter_egg_aktivalva) {
        rajz_szoveg("Easter Egg :)", px, py, SUGO_BETU_MERET,
                    SZIN_EASTER_EGG.r, SZIN_EASTER_EGG.g,
                    SZIN_EASTER_EGG.b);
    }

    szoveg_kozepre("R  UJ JATEK        ESC  FOMENU        F11  TELJES KEPE",
                   rajz_ablak_mag() - ALSO_SUGO_MARGO_JATEK,
                   SUGO_BETU_MERET, SZIN_ALCIM.r, SZIN_ALCIM.g,
                   SZIN_ALCIM.b);
}
