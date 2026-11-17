#ifndef UI_KONSTANSOK_H
#define UI_KONSTANSOK_H

typedef struct {
    float r;
    float g;
    float b;
} SzinRGB;

/* UI szinek. */
static const SzinRGB SZIN_HATT          = {0.07f, 0.08f, 0.11f};
static const SzinRGB SZIN_CIM           = {0.95f, 0.85f, 0.55f};
static const SzinRGB SZIN_ALCIM         = {0.65f, 0.68f, 0.75f};
static const SzinRGB SZIN_GOMB_ALAP     = {0.14f, 0.16f, 0.22f};
static const SzinRGB SZIN_GOMB_AKTIV    = {0.20f, 0.28f, 0.42f};
static const SzinRGB SZIN_GOMB_KERET_A  = {0.75f, 0.65f, 0.35f};
static const SzinRGB SZIN_GOMB_KERET_I  = {0.35f, 0.38f, 0.50f};
static const SzinRGB SZIN_SZOVEG_ALAP   = {0.42f, 0.45f, 0.50f};
static const SzinRGB SZIN_SZOVEG_AKTIV  = {1.00f, 1.00f, 1.00f};
static const SzinRGB SZIN_SZOVEG_SZURKE = {0.85f, 0.85f, 0.85f};
static const SzinRGB SZIN_SZOVEG_ZOLDES = {0.90f, 0.95f, 0.90f};

/* A jatekter szinei. */
static const SzinRGB SZIN_TABLA_VILAGOS   = {0.92f, 0.88f, 0.78f};
static const SzinRGB SZIN_TABLA_SOTET     = {0.42f, 0.48f, 0.55f};
static const SzinRGB SZIN_TABLA_KERET     = {0.20f, 0.16f, 0.10f};
static const SzinRGB SZIN_MEZO_KIJELOLT   = {0.15f, 0.75f, 0.35f};
static const SzinRGB SZIN_MEZO_VALASZT    = {0.95f, 0.80f, 0.15f};
static const SzinRGB SZIN_PANEL_HATT      = {0.13f, 0.10f, 0.14f};
static const SzinRGB SZIN_PANEL_ALSO      = {0.10f, 0.16f, 0.12f};
static const SzinRGB SZIN_NEVJEGY_KOR     = {0.35f, 0.65f, 0.80f};
static const SzinRGB SZIN_LINK            = {0.55f, 0.75f, 1.00f};
static const SzinRGB SZIN_PANEL_SZOVEG    = {0.55f, 0.70f, 0.60f};
static const SzinRGB SZIN_FEHER_MEG       = {0.95f, 0.92f, 0.78f};
static const SzinRGB SZIN_MATT            = {0.95f, 0.35f, 0.35f};
static const SzinRGB SZIN_SZIV            = {0.95f, 0.20f, 0.30f};
static const SzinRGB SZIN_EASTER_EGG      = {0.85f, 0.35f, 0.85f};
static const SzinRGB SZIN_JATEKOS_VILAGOS = {0.95f, 0.95f, 0.95f};
static const SzinRGB SZIN_JATEKOS_SOTET   = {0.35f, 0.35f, 0.35f};

/* Kepernyo-meretek. */
static const float GOMB_W            = 360.0f;
static const float GOMB_H            = 55.0f;
static const float GOMB_KOZ          = 12.0f;
static const float CIM_MERET         = 70.0f;
static const float ALCIM_MERET       = 26.0f;
static const float GOMB_BETU_MERET   = 22.0f;
static const float SUGO_BETU_MERET   = 14.0f;
static const float NEHEZSEG_GOMB_W   = 330.0f;
static const float UI_ALAP_MAGASSAG  = 760.0f;
static const float NEVJEGY_ALAP_MAG  = 800.0f;
static const float PANEL_MARGO_KICSI = 14.0f;
static const float PANEL_MARGO_NAGY  = 24.0f;
static const float PANEL_KOZ_KICSI   = 12.0f;
static const float PANEL_KOZ_NAGY    = 18.0f;
static const int   PANEL_TORZSPONT   = 1100;
static const float PANEL_JOBB_MIN    = 250.0f;
static const float PANEL_JOBB_MAX    = 380.0f;
static const float PANEL_FEJLEC_MAG  = 160.0f;
static const float PANEL_ALSO_SAV    = 40.0f;
static const float TABLA_BELSO_MARGO = 18.0f;

/* AI. */
static const int AI_NEHEZSEG_DB    = 3;
static const int AI_MIN_MELYSEG    = 1;
static const int AI_NORMAL_MELYSEG = 2;

/* Altalanos ertekek. */
static const float KOZEPRE_IGAZIT = 0.5f;
static const float SCALE_ALAP     = 1.0f;
static const float KERET_VEKONY   = 2.0f;
static const float MARGOK_SZORZO  = 2.0f;
static const float KEP_ATMELLETT  = 2.0f;
static const int   NEVJEGY_PAR_DB = 3;
static const int   GOMB_AKTIV    = 1;

/* Elrendezesi tavok. */
static const float FO_MENU_ALCIM_UTANI_KOZ = 50.0f;
static const float FO_MENU_MIN_FELSO_MARGO = 18.0f;
static const float FO_MENU_SCALE_MIN       = 0.72f;
static const float AI_CIM_GOMB_KOZ         = 50.0f;
static const float AI_SUGO_MARGO           = 30.0f;
static const float NEVJEGY_MIN_FELSO_MARGO = 15.0f;
static const float NEVJEGY_SCALE_MIN       = 0.55f;
static const float NEVJEGY_CIMKE_ERTEK_KOZ = 6.0f;
static const float NEVJEGY_GITHUB_CIMKE_KOZ = 8.0f;
static const float NEVJEGY_GITHUB_KOZ      = 60.0f;
static const float NEVJEGY_BLOKK_EXTRA     = 120.0f;
static const float NEVJEGY_SZIV_ELTOLAS    = 18.0f;
static const float ALSO_SUGO_MARGO         = 25.0f;
static const float ALSO_SUGO_MARGO_JATEK   = 34.0f;
static const float KILEPES_CIM_FELSO_ELTOLAS = 80.0f;
static const float KILEPES_CIM_MERET       = 42.0f;
static const float KILEPES_SUGO_ELTOLAS    = 20.0f;
static const float TABLA_ALSO_KOZ          = 6.0f;
static const float PANEL_CIM_FELSO         = 30.0f;
static const float PANEL_ALCIM_FELSO       = 80.0f;
static const float PANEL_VERZIO_FELSO      = 115.0f;
static const float PANEL_CIM_MERET         = 36.0f;

/* UI keretek es betumeretek. */
static const float UI_ALSO_KERET        = 3.0f;
static const float UI_TABLA_KERET       = 4.0f;
static const float UI_TABLA_KERET_KULSO = 8.0f;
static const float UI_TABLA_BETU_ARANY  = 0.24f;
static const float UI_TABLA_BETU_MIN    = 10.0f;
static const float UI_TABLA_BETU_MAX    = 18.0f;
static const float UI_NEHEZSEG_CIM      = 42.0f;
static const float UI_NEVJEGY_KEP_SUGAR = 80.0f;
static const float UI_NEVJEGY_KEP_KOZ   = 30.0f;
static const float UI_NEVJEGY_CIM       = 40.0f;
static const float UI_NEVJEGY_ERTEK     = 24.0f;
static const float UI_NEVJEGY_LINK      = 18.0f;
static const float UI_NEVJEGY_SZIV      = 36.0f;
static const float JOBB_PANEL_ARANY     = 0.27f;
static const float UI_PANEL_BAL_KOZ     = 15.0f;
static const float UI_PANEL_FELSO_KOZ   = 24.0f;
static const float UI_PANEL_SOR_KOZ     = 22.0f;
static const float UI_PANEL_CIMKE_KOZ   = 18.0f;
static const float UI_PANEL_ERTEK_KOZ   = 35.0f;
static const float UI_PANEL_LEPES_KOZ   = 40.0f;
static const float UI_SZOVEG_KICSI      = 12.0f;
static const float UI_SZOVEG_PANEL      = 13.0f;
static const float UI_SZOVEG_LINK       = 16.0f;
static const float UI_SZOVEG_NAGY       = 20.0f;
static const float UI_SZOVEG_JATEK      = 22.0f;
static const float UI_SZOVEG_CIM        = 26.0f;

static const unsigned int TEXTURA_NINCS = 0;

#endif
