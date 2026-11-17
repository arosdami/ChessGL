#ifndef JATEK_RAJZ_H
#define JATEK_RAJZ_H

#include "tabla.h"

typedef struct {
    float x;
    float y;
    float w;
    float h;
} Teglalap;

void jatek_panelek_szamol(Teglalap *bal, Teglalap *jf, Teglalap *ja);
void jatek_tabla_pozicio(const Teglalap *panel, float *tx, float *ty, float *mezo);
void jatek_rajzol(JatekAllapot *j, int kijelolt[MERET][MERET],
                  int easter_egg_aktivalva);

#endif
