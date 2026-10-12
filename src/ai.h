#ifndef AI_H
#define AI_H

#include "tabla.h"

// Az AI kiválasztja a legjobb lépést az adott szín számára.
// Visszatérés: 1 = talált lépést, 0 = nincs lépés
// A *sor_ki, *oszlop_ki, *sor_be, *oszlop_be kimeneti paraméterek.
int ai_lepest_valaszt(JatekAllapot *j, Szin szin,
                      int *sor_ki, int *oszlop_ki,
                      int *sor_be, int *oszlop_be);

#endif