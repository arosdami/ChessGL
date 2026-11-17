#include "babu.h"
#include "rajz.h"

#define BABU_TEXTURA_SOR_FEHER 0
#define BABU_TEXTURA_SOR_FEKETE 1
#define BABU_TEXTURA_OSZLOP_KIRALYNO 0
#define BABU_TEXTURA_OSZLOP_KIRALY 1
#define BABU_TEXTURA_OSZLOP_FUTO 2
#define BABU_TEXTURA_OSZLOP_HUSZAR 3
#define BABU_TEXTURA_OSZLOP_BASTYA 4
#define BABU_TEXTURA_OSZLOP_GYALOG 5
#define BABU_TEXTURA_OSZLOPOK 6
#define BABU_TEXTURA_SOROK 2
#define BABU_NINCS_OSZLOP -1
#define BABU_NINCS_TEXTURA 0
#define BABU_ELSO_OSZLOP 0
#define BABU_UV_KEZDO 0.0f
#define BABU_UV_VEGE 1.0f
#define BABU_TEXTURA_FEHER_ERTEK 1.0f
#define BABU_TEXTURA_FEKETE_ERTEK 0.1f
#define BABU_MEZO_KOZEPE 0.5f
#define BABU_KOR_SUGAR 0.35f
#define BABU_TEXTURA_MERET 0.95f
#define BABU_SZIV_FELE 0.5f
#define BABU_SZIV_KOR_X 0.55f
#define BABU_SZIV_KOR_Y 0.20f
#define BABU_SZIV_HAROMSZOG_ALAP 0.05f
#define BABU_SZIV_CSUCSSZOG 1.30f

/* A babuk texturaja ket sorban hat figurat tartalmaz. */
static unsigned int babuk_textura = 0;

void babu_textura_beallit(unsigned int tex) {
    babuk_textura = tex;
}

static void babu_cella(Babu b, int *sor, int *oszlop) {
    *sor = (b.szin == FEHER) ? BABU_TEXTURA_SOR_FEHER : BABU_TEXTURA_SOR_FEKETE;

    switch (b.tipus) {
        case KIRALYNO: *oszlop = BABU_TEXTURA_OSZLOP_KIRALYNO; break;
        case KIRALY:   *oszlop = BABU_TEXTURA_OSZLOP_KIRALY; break;
        case FUTO:     *oszlop = BABU_TEXTURA_OSZLOP_FUTO; break;
        case HUSZAR:   *oszlop = BABU_TEXTURA_OSZLOP_HUSZAR; break;
        case BASTYA:   *oszlop = BABU_TEXTURA_OSZLOP_BASTYA; break;
        case GYALOG:   *oszlop = BABU_TEXTURA_OSZLOP_GYALOG; break;
        default:       *oszlop = BABU_NINCS_OSZLOP; break;
    }
}

void babu_rajzol(Babu b, float x, float y, float mezo) {
    if (b.tipus == URES) return;

    int sor, oszlop;
    babu_cella(b, &sor, &oszlop);
    if (oszlop < BABU_ELSO_OSZLOP) return;

    if (babuk_textura == BABU_NINCS_TEXTURA) {
        float v = (b.szin == FEHER) ? BABU_TEXTURA_FEHER_ERTEK
                                     : BABU_TEXTURA_FEKETE_ERTEK;
        rajz_kor(x + mezo * BABU_MEZO_KOZEPE,
                 y + mezo * BABU_MEZO_KOZEPE,
                 mezo * BABU_KOR_SUGAR, v, v, v);
        return;
    }

    float cella_w = BABU_UV_VEGE / BABU_TEXTURA_OSZLOPOK;
    float cella_h = BABU_UV_VEGE / BABU_TEXTURA_SOROK;

    float u0 = BABU_UV_KEZDO + oszlop * cella_w;
    float u1 = u0 + cella_w;
    float v0 = BABU_UV_KEZDO + sor * cella_h;
    float v1 = v0 + cella_h;

    float belso = mezo * BABU_TEXTURA_MERET;
    float eltolas = (mezo - belso) * BABU_MEZO_KOZEPE;

    rajz_textura_negyszog(babuk_textura,
                          x + eltolas, y + eltolas,
                          belso, belso,
                          u0, v0, u1, v1);
}

void sziv_rajzol(float cx, float cy, float meret, float r, float g, float b) {
    float fel = meret * BABU_SZIV_FELE;
    rajz_kor(cx - fel * BABU_SZIV_KOR_X,
             cy - fel * BABU_SZIV_KOR_Y,
             fel * BABU_SZIV_KOR_X, r, g, b);
    rajz_kor(cx + fel * BABU_SZIV_KOR_X,
             cy - fel * BABU_SZIV_KOR_Y,
             fel * BABU_SZIV_KOR_X, r, g, b);
    rajz_haromszog(cx - fel, cy + fel * BABU_SZIV_HAROMSZOG_ALAP,
                   cx + fel, cy + fel * BABU_SZIV_HAROMSZOG_ALAP,
                   cx,       cy + fel * BABU_SZIV_CSUCSSZOG,
                   r, g, b);
}
