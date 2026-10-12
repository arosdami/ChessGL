#include "menu.h"

static Kepernyo aktualis = KEPERNYO_FOMENU;
static JatekMod jatekmod = MOD_KET_JATEKOS;

void menu_inicializalas(void) {
    aktualis = KEPERNYO_FOMENU;
    jatekmod = MOD_KET_JATEKOS;
}

Kepernyo menu_aktualis(void) {
    return aktualis;
}

void menu_valtas(Kepernyo k) {
    aktualis = k;
}

void menu_jatekmod_beallit(JatekMod m) { jatekmod = m; }
JatekMod menu_jatekmod(void) { return jatekmod; }