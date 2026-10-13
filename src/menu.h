#ifndef MENU_H
#define MENU_H

typedef enum {
    KEPERNYO_FOMENU = 0,
    KEPERNYO_JATEK,
    KEPERNYO_NEVJEGY,
    KEPERNYO_KILEPES,
    KEPERNYO_AI_NEHEZSEG
} Kepernyo;

// Játékmód
typedef enum {
    MOD_KET_JATEKOS = 0,   // két ember
    MOD_GEP_ELLEN          // AI ellen
} JatekMod;

void menu_inicializalas(void);
Kepernyo menu_aktualis(void);
void menu_valtas(Kepernyo k);

void menu_jatekmod_beallit(JatekMod m);
JatekMod menu_jatekmod(void);

#endif