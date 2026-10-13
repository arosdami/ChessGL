#include "ai.h"
#include <stdlib.h>
#include <string.h>

#define AI_ALAP_MELYSEG 2
#define AI_MIN_MELYSEG  1
#define AI_MAX_MELYSEG  3
#define AI_MIN_PONT      -100000
#define AI_MAX_PONT       100000
#define AI_MATT_PONT      90000

static int ai_melyseg = AI_ALAP_MELYSEG;

void ai_nehezseg_beallit(int melyseg) {
    if (melyseg < AI_MIN_MELYSEG) melyseg = AI_MIN_MELYSEG;
    if (melyseg > AI_MAX_MELYSEG) melyseg = AI_MAX_MELYSEG;
    ai_melyseg = melyseg;
}

int ai_nehezseg(void) {
    return ai_melyseg;
}

// ---- Bábu értékek ----
static int babu_ertek(BabuTipus t) {
    switch (t) {
        case GYALOG:   return 100;
        case HUSZAR:   return 320;
        case FUTO:     return 330;
        case BASTYA:   return 500;
        case KIRALYNO: return 900;
        case KIRALY:   return 20000;
        default:       return 0;
    }
}

// A tábla kiértékelése adott szemszögből.
// Ha feher_szemszog = 1, akkor a fehér állását adja vissza pozitív számmal.
static int tabla_ertek(JatekAllapot *j, Szin sajat) {
    int pont = 0;
    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            Babu b = j->tabla[s][o];
            if (b.tipus == URES) continue;
            int ertek = babu_ertek(b.tipus);
            if (b.szin == sajat) pont += ertek;
            else                 pont -= ertek;
        }
    }
    return pont;
}

// Lépésvégrehajtás "szimulációban" – a memóriabeli állapotot vissza is állítjuk.
// Visszatérés: a levett bábu (hogy vissza tudjuk tenni).
static Babu lepes_szimulal(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu le = j->tabla[cr][co];
    j->tabla[cr][co] = j->tabla[sr][so];
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};
    return le;
}

static void lepes_visszaallit(JatekAllapot *j, int sr, int so, int cr, int co, Babu le) {
    j->tabla[sr][so] = j->tabla[cr][co];
    j->tabla[cr][co] = le;
}

// Minimax alpha-beta vágással.
// Melyseg: hány lépést néz előre.
// Visszatérés: a lépés értéke az AI szemszögéből.
static int minimax(JatekAllapot *j, int melyseg, int alfa, int beta, Szin ai_szin) {
    // Ha elértük a mélységet, kiértékeljük
    if (melyseg == 0) {
        return tabla_ertek(j, ai_szin);
    }

    // Ki lép most?
    Szin lepo = j->kovetkezo;
    int maximalizal = (lepo == ai_szin);

    // Összes legális lépés
    int legjobb = maximalizal ? AI_MIN_PONT : AI_MAX_PONT;

    for (int sr = 0; sr < MERET; sr++) {
        for (int so = 0; so < MERET; so++) {
            Babu b = j->tabla[sr][so];
            if (b.tipus == URES || b.szin != lepo) continue;

            for (int cr = 0; cr < MERET; cr++) {
                for (int co = 0; co < MERET; co++) {
                    // Szimuláljuk: a babu_szabalyos_lepes a jelenlegi állapotot nézi,
                    // de nekünk a kovetkezo szint már a másik játékosé.
                    // Ezért ideiglenesen beállítjuk a kovetkezo színt.
                    Szin mentett = j->kovetkezo;
                    j->kovetkezo = lepo;

                    int ervenyes = babu_szabalyos_lepes(j, sr, so, cr, co);
                    if (!ervenyes) {
                        j->kovetkezo = mentett;
                        continue;
                    }

                    // Lépés szimulálása
                    Babu le = lepes_szimulal(j, sr, so, cr, co);
                    j->kovetkezo = (lepo == FEHER) ? FEKETE : FEHER;

                    int ertek = minimax(j, melyseg - 1, alfa, beta, ai_szin);

                    // Visszaállítás
                    j->kovetkezo = lepo;
                    lepes_visszaallit(j, sr, so, cr, co, le);
                    j->kovetkezo = mentett;

                    if (maximalizal) {
                        if (ertek > legjobb) legjobb = ertek;
                        if (ertek > alfa) alfa = ertek;
                        if (beta <= alfa) return legjobb;  // vágás
                    } else {
                        if (ertek < legjobb) legjobb = ertek;
                        if (ertek < beta) beta = ertek;
                        if (beta <= alfa) return legjobb;  // vágás
                    }
                }
            }
        }
    }

    // Ha nincs legális lépés (matt vagy patt)
    if (legjobb == AI_MIN_PONT) return -AI_MATT_PONT;
    if (legjobb == AI_MAX_PONT) return AI_MATT_PONT;
    return legjobb;
}

int ai_lepest_valaszt(JatekAllapot *j, Szin szin,
                      int *sor_ki, int *oszlop_ki,
                      int *sor_be, int *oszlop_be) {

    int legjobb_ertek = -AI_MAX_PONT * 10;
    int talalt = 0;

    Szin mentett = j->kovetkezo;
    j->kovetkezo = szin;

    for (int sr = 0; sr < MERET; sr++) {
        for (int so = 0; so < MERET; so++) {
            Babu b = j->tabla[sr][so];
            if (b.tipus == URES || b.szin != szin) continue;

            for (int cr = 0; cr < MERET; cr++) {
                for (int co = 0; co < MERET; co++) {
                    if (!babu_szabalyos_lepes(j, sr, so, cr, co)) continue;

                    // Lépés szimulálása
                    Babu le = lepes_szimulal(j, sr, so, cr, co);
                    j->kovetkezo = (szin == FEHER) ? FEKETE : FEHER;

                    int ertek = minimax(j, ai_melyseg,
                                        -AI_MAX_PONT * 10, AI_MAX_PONT * 10, szin);

                    // Visszaállítás
                    j->kovetkezo = szin;
                    lepes_visszaallit(j, sr, so, cr, co, le);

                    if (!talalt || ertek > legjobb_ertek) {
                        legjobb_ertek = ertek;
                        *sor_ki = sr;
                        *oszlop_ki = so;
                        *sor_be = cr;
                        *oszlop_be = co;
                        talalt = 1;
                    }
                }
            }
        }
    }

    j->kovetkezo = mentett;
    return talalt;
}