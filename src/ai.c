#include "ai.h"

static const int AI_ALAP_MELYSEG = 1;
static const int AI_MIN_MELYSEG = 1;
static const int AI_MAX_MELYSEG = 3;
static const int AI_VEGTELEN = 1000000;

static const int AI_PONT_GYALOG = 100;
static const int AI_PONT_HUSZAR = 320;
static const int AI_PONT_FUTO = 330;
static const int AI_PONT_BASTYA = 500;
static const int AI_PONT_KIRALYNO = 900;
static const int AI_PONT_KIRALY = 20000;

static int ai_melyseg = AI_ALAP_MELYSEG;

void ai_nehezseg_beallit(int melyseg) {
    if (melyseg < AI_MIN_MELYSEG) melyseg = AI_MIN_MELYSEG;
    if (melyseg > AI_MAX_MELYSEG) melyseg = AI_MAX_MELYSEG;
    ai_melyseg = melyseg;
}

int ai_nehezseg(void) {
    return ai_melyseg;
}

static int babu_ertek(BabuTipus tipus) {
    switch (tipus) {
        case GYALOG:   return AI_PONT_GYALOG;
        case HUSZAR:   return AI_PONT_HUSZAR;
        case FUTO:     return AI_PONT_FUTO;
        case BASTYA:   return AI_PONT_BASTYA;
        case KIRALYNO: return AI_PONT_KIRALYNO;
        case KIRALY:   return AI_PONT_KIRALY;
        default:       return 0;
    }
}

static int tabla_ertek(const JatekAllapot *j, Szin ai_szin) {
    int pont = 0;

    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            Babu b = j->tabla[s][o];
            int ertek = babu_ertek(b.tipus);
            if (b.szin == ai_szin) pont += ertek;
            else if (b.szin != NINCS_SZIN) pont -= ertek;
        }
    }
    return pont;
}

static Babu lepest_alkalmaz(JatekAllapot *j,
                            int sr, int so, int cr, int co) {
    Babu levett = j->tabla[cr][co];
    j->tabla[cr][co] = j->tabla[sr][so];
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};
    return levett;
}

static void lepest_visszavon(JatekAllapot *j,
                             int sr, int so, int cr, int co,
                             Babu levett) {
    j->tabla[sr][so] = j->tabla[cr][co];
    j->tabla[cr][co] = levett;
}

static int minimax(JatekAllapot *j, int melyseg,
                   int alfa, int beta, Szin ai_szin) {
    if (melyseg == 0) return tabla_ertek(j, ai_szin);

    int maximalizal = j->kovetkezo == ai_szin;
    int legjobb = maximalizal ? -AI_VEGTELEN : AI_VEGTELEN;
    int talalt = 0;
    int lehetseges[MERET][MERET];

    for (int sr = 0; sr < MERET; sr++) {
        for (int so = 0; so < MERET; so++) {
            Babu b = j->tabla[sr][so];
            if (b.tipus == URES || b.szin != j->kovetkezo) continue;

            lepesek_listazasa(j, sr, so, lehetseges);
            for (int cr = 0; cr < MERET; cr++) {
                for (int co = 0; co < MERET; co++) {
                    if (!lehetseges[cr][co]) continue;

                    Babu levett = lepest_alkalmaz(j, sr, so, cr, co);
                    Szin mentett_szin = j->kovetkezo;
                    j->kovetkezo = (mentett_szin == FEHER) ? FEKETE : FEHER;

                    int pont = minimax(j, melyseg - 1,
                                       alfa, beta, ai_szin);

                    j->kovetkezo = mentett_szin;
                    lepest_visszavon(j, sr, so, cr, co, levett);
                    talalt = 1;

                    if (maximalizal) {
                        if (pont > legjobb) legjobb = pont;
                        if (legjobb > alfa) alfa = legjobb;
                    } else {
                        if (pont < legjobb) legjobb = pont;
                        if (legjobb < beta) beta = legjobb;
                    }

                    if (beta <= alfa) return legjobb;
                }
            }
        }
    }

    return talalt ? legjobb : tabla_ertek(j, ai_szin);
}

int ai_lepest_valaszt(JatekAllapot *j, Szin szin,
                      int *sor_ki, int *oszlop_ki,
                      int *sor_be, int *oszlop_be) {
    Szin mentett_szin = j->kovetkezo;
    int legjobb_pont = -AI_VEGTELEN;
    int talalt = 0;
    int lehetseges[MERET][MERET];

    j->kovetkezo = szin;

    for (int sr = 0; sr < MERET; sr++) {
        for (int so = 0; so < MERET; so++) {
            Babu b = j->tabla[sr][so];
            if (b.tipus == URES || b.szin != szin) continue;

            lepesek_listazasa(j, sr, so, lehetseges);
            for (int cr = 0; cr < MERET; cr++) {
                for (int co = 0; co < MERET; co++) {
                    if (!lehetseges[cr][co]) continue;

                    Babu levett = lepest_alkalmaz(j, sr, so, cr, co);
                    j->kovetkezo = (szin == FEHER) ? FEKETE : FEHER;
                    int pont = minimax(j, ai_melyseg - AI_MIN_MELYSEG,
                                       -AI_VEGTELEN, AI_VEGTELEN, szin);
                    j->kovetkezo = szin;
                    lepest_visszavon(j, sr, so, cr, co, levett);

                    if (!talalt || pont > legjobb_pont) {
                        legjobb_pont = pont;
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

    j->kovetkezo = mentett_szin;
    return talalt;
}
