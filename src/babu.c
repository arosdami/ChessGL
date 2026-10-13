#include "babu.h"
#include "rajz.h"

// A resources/babuk.png egy 6 cellas sorokat tartalmazo
// sprite sheet. Minden sorban 6 babu van, balrol jobbra:
//   0: kiralyno, 1: kiraly, 2: futo, 3: huszar, 4: bastya, 5: gyalog
//
// Felso sor (0): feher babuk
// Also sor (1):  fekete babuk
static unsigned int babuk_textura = 0;

void babu_textura_beallit(unsigned int tex) {
    babuk_textura = tex;
}

static void babu_cella(Babu b, int *sor, int *oszlop) {
    // FELSO sor = FEHER (vilagos), ALSO sor = FEKETE (sotet)
    *sor = (b.szin == FEHER) ? 0 : 1;

    switch (b.tipus) {
        case KIRALYNO: *oszlop = 0; break;
        case KIRALY:   *oszlop = 1; break;
        case FUTO:     *oszlop = 2; break;
        case HUSZAR:   *oszlop = 3; break;
        case BASTYA:   *oszlop = 4; break;
        case GYALOG:   *oszlop = 5; break;
        default:       *oszlop = -1; break;
    }
}

void babu_rajzol(Babu b, float x, float y, float mezo) {
    if (b.tipus == URES) return;

    int sor, oszlop;
    babu_cella(b, &sor, &oszlop);
    if (oszlop < 0) return;

    if (babuk_textura == 0) {
        float v = (b.szin == FEHER) ? 1.0f : 0.1f;
        rajz_kor(x + mezo*0.5f, y + mezo*0.5f, mezo*0.35f, v, v, v);
        return;
    }

    // 6 oszlop x 2 sor
    float cella_w = 1.0f / 6.0f;
    float cella_h = 1.0f / 2.0f;

    float u0 = oszlop * cella_w;
    float u1 = u0 + cella_w;
    float v0 = sor * cella_h;
    float v1 = v0 + cella_h;

    float belso = mezo * 0.95f;
    float eltolas = (mezo - belso) * 0.5f;

    rajz_textura_negyszog(babuk_textura,
                          x + eltolas, y + eltolas,
                          belso, belso,
                          u0, v0, u1, v1);
}

void sziv_rajzol(float cx, float cy, float meret, float r, float g, float b) {
    float fel = meret * 0.5f;
    rajz_kor(cx - fel * 0.55f, cy - fel * 0.20f, fel * 0.55f, r, g, b);
    rajz_kor(cx + fel * 0.55f, cy - fel * 0.20f, fel * 0.55f, r, g, b);
    rajz_haromszog(cx - fel, cy + fel * 0.05f,
                   cx + fel, cy + fel * 0.05f,
                   cx,       cy + fel * 1.30f,
                   r, g, b);
}
