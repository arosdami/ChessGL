#ifndef AI_H
#define AI_H

#include "tabla.h"

/* A megadott szinhez legjobbnek talalt lepes kivalasztasa. */
int ai_lepest_valaszt(JatekAllapot *j, Szin szin,
                      int *sor_ki, int *oszlop_ki,
                      int *sor_be, int *oszlop_be);

void ai_nehezseg_beallit(int melyseg);
int ai_nehezseg(void);

#endif