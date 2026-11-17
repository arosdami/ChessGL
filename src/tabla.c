#include "tabla.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLA_FEHER_IRANY -1
#define TABLA_FEKETE_IRANY 1
#define TABLA_DUPLA_LEPES 2
#define TABLA_KIRALY_LEPES 1
#define TABLA_ROVID_SANC_CEL 6
#define TABLA_HOSSZU_SANC_CEL 2
#define TABLA_BASTYA_H_OSZLOP 7
#define TABLA_BASTYA_A_OSZLOP 0
#define TABLA_ROVID_SANC_KOZTES 5
#define TABLA_HOSSZU_SANC_KOZTES_1 1
#define TABLA_HOSSZU_SANC_KOZTES_2 2
#define TABLA_HOSSZU_SANC_KOZTES_3 3
#define TABLA_BASTYA_H_CEL 5
#define TABLA_BASTYA_A_CEL 3
#define TABLA_SAKK_SOR_ALAP '8'

/* Sakklogikai segedfuggvenyek. */
static Szin ellenfel(Szin s) {
    return (s == FEHER) ? FEKETE : FEHER;
}

static int ervenyes(int sor, int oszlop) {
    return sor >= 0 && sor < MERET && oszlop >= 0 && oszlop < MERET;
}

static int ures_mezot_ellenoriz(JatekAllapot *j, int sor, int oszlop) {
    return j->tabla[sor][oszlop].tipus == URES;
}

static int sajat_babu_e(JatekAllapot *j, int sor, int oszlop, Szin szin) {
    return j->tabla[sor][oszlop].tipus != URES &&
           j->tabla[sor][oszlop].szin == szin;
}

static int ellenfel_babu_e(JatekAllapot *j, int sor, int oszlop, Szin szin) {
    return j->tabla[sor][oszlop].tipus != URES &&
           j->tabla[sor][oszlop].szin != szin;
}

/* A kezdoallapot beallitasa. */
void tabla_inicializalas(JatekAllapot *j) {
    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            j->tabla[s][o] = (Babu){URES, NINCS_SZIN};

    BabuTipus hatso[MERET] = {
        BASTYA, HUSZAR, FUTO, KIRALYNO, KIRALY, FUTO, HUSZAR, BASTYA
    };

    for (int o = 0; o < MERET; o++) {
        j->tabla[TABLA_SOR_FEKETE_FIGURA][o] = (Babu){hatso[o], FEKETE};
        j->tabla[TABLA_SOR_FEKETE_GYALOG][o] = (Babu){GYALOG, FEKETE};
        j->tabla[TABLA_SOR_FEHER_GYALOG][o] = (Babu){GYALOG, FEHER};
        j->tabla[TABLA_SOR_FEHER_FIGURA][o] = (Babu){hatso[o], FEHER};
    }

    j->kovetkezo = FEHER;
    j->kivalasztott_sor   = TABLA_NINCS_MEZO;
    j->kivalasztott_oszlop= TABLA_NINCS_MEZO;
    j->van_kivalasztva    = 0;
    j->jatek_vege         = JATEK_FOLYAMATBAN;
    strcpy(j->utolso_lepes, "-");

    j->feher_kiraly_mozdult    = 0;
    j->fekete_kiraly_mozdult   = 0;
    j->feher_bastya_a_mozdult  = 0;
    j->feher_bastya_h_mozdult  = 0;
    j->fekete_bastya_a_mozdult = 0;
    j->fekete_bastya_h_mozdult = 0;

    j->en_passant_sor    = TABLA_NINCS_MEZO;
    j->en_passant_oszlop = TABLA_NINCS_MEZO;
}

/* Gyaloglepesek. */
static int gyalog_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu b = j->tabla[sr][so];
    int irany     = (b.szin == FEHER) ? TABLA_FEHER_IRANY : TABLA_FEKETE_IRANY;
    int kezdo_sor = (b.szin == FEHER) ? TABLA_SOR_FEHER_GYALOG
                                      : TABLA_SOR_FEKETE_GYALOG;
    int ds        = cr - sr;
    int dob       = co - so;

    /* Egyenes lepes. */
    if (dob == 0) {
        if (ds == irany && ures_mezot_ellenoriz(j, cr, co)) return 1;
        if (sr == kezdo_sor && ds == TABLA_DUPLA_LEPES * irany &&
            ures_mezot_ellenoriz(j, sr + irany, so) &&
            ures_mezot_ellenoriz(j, cr, co)) return 1;
        return 0;
    }

    /* Atlos utes. */
    if (abs(dob) == TABLA_KIRALY_LEPES && ds == irany) {
        /* Normal utes. */
        if (ellenfel_babu_e(j, cr, co, b.szin)) return 1;

        /* En passant. */
        /* A celmezo ures, ezert csak az utolso dupla gyaloglepes igazolhatja. */
        if (j->en_passant_sor == cr && j->en_passant_oszlop == co) {
            return 1;
        }
    }
    return 0;
}

/* Bastyalepesek. */
static int bastya_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (sr != cr && so != co) return 0;
    int ds  = (cr > sr) - (cr < sr);
    int dob = (co > so) - (co < so);
    int s = sr + ds, o = so + dob;
    while (s != cr || o != co) {
        if (j->tabla[s][o].tipus != URES) return 0;
        s += ds; o += dob;
    }
    return 1;
}

/* Futolepesek. */
static int futo_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (abs(cr - sr) != abs(co - so)) return 0;
    int ds  = (cr > sr) - (cr < sr);
    int dob = (co > so) - (co < so);
    int s = sr + ds, o = so + dob;
    while (s != cr && o != co) {
        if (j->tabla[s][o].tipus != URES) return 0;
        s += ds; o += dob;
    }
    return 1;
}

/* Huszarlepesek. */
static int huszar_szabalyos(int sr, int so, int cr, int co) {
    int ds  = abs(cr - sr);
    int dob = abs(co - so);
    return (ds == TABLA_DUPLA_LEPES && dob == TABLA_KIRALY_LEPES) ||
           (ds == TABLA_KIRALY_LEPES && dob == TABLA_DUPLA_LEPES);
}

/* Kiralynoi lepesek. */
static int kiralyno_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    return bastya_szabalyos(j, sr, so, cr, co) ||
           futo_szabalyos(j, sr, so, cr, co);
}

/* Kiralylepesek es sancolas. */
static int kiraly_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu b = j->tabla[sr][so];

    if (abs(cr - sr) <= TABLA_KIRALY_LEPES &&
        abs(co - so) <= TABLA_KIRALY_LEPES &&
        !(sr == cr && so == co)) {
        if (sajat_babu_e(j, cr, co, b.szin)) return 0;
        return 1;
    }

    if (sr == cr && abs(co - so) == TABLA_DUPLA_LEPES) {
        if (b.szin == FEHER && j->feher_kiraly_mozdult) return 0;
        if (b.szin == FEKETE && j->fekete_kiraly_mozdult) return 0;

        if (co == TABLA_ROVID_SANC_CEL) {
            int bastya_oszlop = TABLA_BASTYA_H_OSZLOP;
            int bastya_mozdult = (b.szin == FEHER)
                                 ? j->feher_bastya_h_mozdult
                                 : j->fekete_bastya_h_mozdult;
            if (bastya_mozdult) return 0;
            if (!ures_mezot_ellenoriz(j, sr, TABLA_ROVID_SANC_KOZTES)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, TABLA_ROVID_SANC_CEL)) return 0;
            if (j->tabla[sr][bastya_oszlop].tipus != BASTYA) return 0;
            if (j->tabla[sr][bastya_oszlop].szin != b.szin) return 0;
            return 1;
        }

        if (co == TABLA_HOSSZU_SANC_CEL) {
            int bastya_oszlop = TABLA_BASTYA_A_OSZLOP;
            int bastya_mozdult = (b.szin == FEHER)
                                 ? j->feher_bastya_a_mozdult
                                 : j->fekete_bastya_a_mozdult;
            if (bastya_mozdult) return 0;
            if (!ures_mezot_ellenoriz(j, sr, TABLA_HOSSZU_SANC_KOZTES_1)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, TABLA_HOSSZU_SANC_KOZTES_2)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, TABLA_HOSSZU_SANC_KOZTES_3)) return 0;
            if (j->tabla[sr][bastya_oszlop].tipus != BASTYA) return 0;
            if (j->tabla[sr][bastya_oszlop].szin != b.szin) return 0;
            return 1;
        }
    }

    return 0;
}

/* Sakkvizsgalat. */
/* Megadja, hogy egy mezot tamad-e az adott szin. */
static int mezot_tamadja(JatekAllapot *j, int cel_sor, int cel_oszlop,
                         Szin tamado, int sajat_kizaras) {
    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            Babu b = j->tabla[s][o];
            if (b.tipus == URES || b.szin != tamado) continue;
            if (sajat_kizaras && s == cel_sor && o == cel_oszlop) continue;

            Szin mentett = j->kovetkezo;
            j->kovetkezo = tamado;

            int tamad = 0;
            int ds = cel_sor - s, dob = cel_oszlop - o;

            switch (b.tipus) {
                case GYALOG:
                    if (abs(dob) == 1) {
                        int irany = (b.szin == FEHER)
                                  ? TABLA_FEHER_IRANY : TABLA_FEKETE_IRANY;
                        if (ds == irany) tamad = 1;
                    }
                    break;
                case BASTYA:   tamad = bastya_szabalyos(j, s, o, cel_sor, cel_oszlop); break;
                case HUSZAR:   tamad = huszar_szabalyos(s, o, cel_sor, cel_oszlop); break;
                case FUTO:     tamad = futo_szabalyos(j, s, o, cel_sor, cel_oszlop); break;
                case KIRALYNO: tamad = kiralyno_szabalyos(j, s, o, cel_sor, cel_oszlop); break;
                case KIRALY:   tamad = (abs(ds) <= 1 && abs(dob) <= 1 && (ds || dob)); break;
                default: break;
            }

            j->kovetkezo = mentett;
            if (tamad) return 1;
        }
    }
    return 0;
}

static void kiraly_helye(JatekAllapot *j, Szin szin, int *sor, int *oszlop) {
    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            if (j->tabla[s][o].tipus == KIRALY && j->tabla[s][o].szin == szin) {
                *sor = s;
                *oszlop = o;
                return;
            }
    *sor = -1;
    *oszlop = -1;
}

int sakkban_van_e(JatekAllapot *j, Szin szin) {
    int ks, ko;
    kiraly_helye(j, szin, &ks, &ko);
    if (ks < 0) return 0;
    return mezot_tamadja(j, ks, ko, ellenfel(szin), 0);
}

/* Altalanos lepesellenorzes. */
int babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!ervenyes(sr, so) || !ervenyes(cr, co)) return 0;
    Babu b = j->tabla[sr][so];
    if (b.tipus == URES) return 0;
    if (b.szin != j->kovetkezo) return 0;
    if (sr == cr && so == co) return 0;

    if (sajat_babu_e(j, cr, co, b.szin)) return 0;

    int szabalyos = 0;
    switch (b.tipus) {
        case GYALOG:   szabalyos = gyalog_szabalyos(j, sr, so, cr, co); break;
        case BASTYA:   szabalyos = bastya_szabalyos(j, sr, so, cr, co); break;
        case HUSZAR:   szabalyos = huszar_szabalyos(sr, so, cr, co); break;
        case FUTO:     szabalyos = futo_szabalyos(j, sr, so, cr, co); break;
        case KIRALYNO: szabalyos = kiralyno_szabalyos(j, sr, so, cr, co); break;
        case KIRALY:   szabalyos = kiraly_szabalyos(j, sr, so, cr, co); break;
        default: break;
    }
    if (!szabalyos) return 0;

    /* A lepes ideiglenes alkalmazasaval ellenorizzuk a sajat kiralyt. */
    Babu le = j->tabla[cr][co];
    Babu mozgo = j->tabla[sr][so];
    j->tabla[cr][co] = mozgo;
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};

    int sajat_sakk = sakkban_van_e(j, b.szin);

    j->tabla[sr][so] = mozgo;
    j->tabla[cr][co] = le;

    if (sajat_sakk) return 0;
    return 1;
}

/* Egy lepes vegrehajtasa. */
int lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!babu_szabalyos_lepes(j, sr, so, cr, co)) return 0;

    Babu b = j->tabla[sr][so];

    int en_passant = 0;
    if (b.tipus == GYALOG && abs(co - so) == TABLA_KIRALY_LEPES &&
        j->tabla[cr][co].tipus == URES) {
        j->tabla[sr][co] = (Babu){URES, NINCS_SZIN};
        en_passant = 1;
    }

    int sancolas = 0;
    if (b.tipus == KIRALY && abs(co - so) == TABLA_DUPLA_LEPES) {
        sancolas = 1;
        if (co == TABLA_ROVID_SANC_CEL) {
            j->tabla[sr][TABLA_BASTYA_H_CEL] =
                j->tabla[sr][TABLA_BASTYA_H_OSZLOP];
            j->tabla[sr][TABLA_BASTYA_H_OSZLOP] = (Babu){URES, NINCS_SZIN};
        } else if (co == TABLA_HOSSZU_SANC_CEL) {
            j->tabla[sr][TABLA_BASTYA_A_CEL] =
                j->tabla[sr][TABLA_BASTYA_A_OSZLOP];
            j->tabla[sr][TABLA_BASTYA_A_OSZLOP] = (Babu){URES, NINCS_SZIN};
        }
    }

    if (b.tipus == GYALOG &&
        (cr == TABLA_SOR_FEKETE_FIGURA || cr == TABLA_SOR_FEHER_FIGURA)) {
        b.tipus = KIRALYNO;
    }

    if (b.tipus == KIRALY) {
        if (b.szin == FEHER) j->feher_kiraly_mozdult = 1;
        else                 j->fekete_kiraly_mozdult = 1;
    }
    if (b.tipus == BASTYA) {
        if (b.szin == FEHER && so == TABLA_BASTYA_A_OSZLOP) j->feher_bastya_a_mozdult = 1;
        if (b.szin == FEHER && so == TABLA_BASTYA_H_OSZLOP) j->feher_bastya_h_mozdult = 1;
        if (b.szin == FEKETE && so == TABLA_BASTYA_A_OSZLOP) j->fekete_bastya_a_mozdult = 1;
        if (b.szin == FEKETE && so == TABLA_BASTYA_H_OSZLOP) j->fekete_bastya_h_mozdult = 1;
    }

    j->tabla[cr][co] = b;
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};

    j->en_passant_sor    = TABLA_NINCS_MEZO;
    j->en_passant_oszlop = TABLA_NINCS_MEZO;
    if (b.tipus == GYALOG && abs(cr - sr) == TABLA_DUPLA_LEPES) {
        j->en_passant_sor    = (sr + cr) / 2;
        j->en_passant_oszlop = so;
    }

    const char *oszlop_nev = "abcdefgh";
    char szin_jel = (b.szin == FEHER) ? 'F' : 'S';
    char tipus_jel = '?';
    switch (b.tipus) {
        case GYALOG:   tipus_jel = ' '; break;
        case BASTYA:   tipus_jel = 'B'; break;
        case HUSZAR:   tipus_jel = 'H'; break;
        case FUTO:     tipus_jel = 'F'; break;
        case KIRALYNO: tipus_jel = 'Q'; break;
        case KIRALY:   tipus_jel = 'K'; break;
        default: break;
    }
    char specialis = ' ';
    if (en_passant) specialis = 'e';
    if (sancolas)   specialis = (co == TABLA_ROVID_SANC_CEL) ? 'r' : 'l';

    snprintf(j->utolso_lepes, sizeof(j->utolso_lepes),
             "%c%c%c%c-%c%c%c",
             szin_jel, tipus_jel, oszlop_nev[so],
             TABLA_SAKK_SOR_ALAP - sr, oszlop_nev[co],
             TABLA_SAKK_SOR_ALAP - cr, specialis);

    j->kovetkezo = ellenfel(j->kovetkezo);

    if (!van_legalis_lepes(j, j->kovetkezo)) {
        if (sakkban_van_e(j, j->kovetkezo)) {
            j->jatek_vege = JATEK_MATT;
        } else {
            j->jatek_vege = JATEK_PATT;
        }
    }

    j->van_kivalasztva     = 0;
    j->kivalasztott_sor    = TABLA_NINCS_MEZO;
    j->kivalasztott_oszlop = TABLA_NINCS_MEZO;
    return 1;
}

/* A valaszthato celmezok listazasa. */
void lepesek_listazasa(JatekAllapot *j, int sor, int oszlop, int tomb[MERET][MERET]) {
    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            tomb[s][o] = 0;

    if (!ervenyes(sor, oszlop)) return;
    if (j->tabla[sor][oszlop].szin != j->kovetkezo) return;

    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            if (babu_szabalyos_lepes(j, sor, oszlop, s, o))
                tomb[s][o] = 1;
}

int van_legalis_lepes(JatekAllapot *j, Szin szin) {
    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            Babu b = j->tabla[s][o];
            if (b.tipus == URES || b.szin != szin) continue;
            for (int cs = 0; cs < MERET; cs++)
                for (int co = 0; co < MERET; co++)
                    if (babu_szabalyos_lepes(j, s, o, cs, co))
                        return 1;
        }
    }
    return 0;
}
