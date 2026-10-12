#include "tabla.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Seg?d: ellenf?l sz?ne
static Szin ellenfel(Szin s) {
    return (s == FEHER) ? FEKETE : FEHER;
}

// Seg?d: ?rv?nyes koordin?ta-e?
static int ervenyes(int sor, int oszlop) {
    return sor >= 0 && sor < MERET && oszlop >= 0 && oszlop < MERET;
}

// T?bla alaphelyzetbe ?ll?t?sa
void tabla_inicializalas(JatekAllapot *j) {
    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            j->tabla[s][o] = (Babu){URES, NINCS_SZIN};

    BabuTipus hatso[MERET] = {
        BASTYA, HUSZAR, FUTO, KIRALYNO, KIRALY, FUTO, HUSZAR, BASTYA
    };

    for (int o = 0; o < MERET; o++) {
        j->tabla[0][o] = (Babu){hatso[o], FEKETE};
        j->tabla[1][o] = (Babu){GYALOG,   FEKETE};
        j->tabla[6][o] = (Babu){GYALOG,   FEHER};
        j->tabla[7][o] = (Babu){hatso[o], FEHER};
    }

    j->kovetkezo          = FEHER;
    j->kivalasztott_sor   = -1;
    j->kivalasztott_oszlop= -1;
    j->van_kivalasztva    = 0;
    j->jatek_vege         = 0;
    strcpy(j->utolso_lepes, "-");
}

// Gyalog
static int gyalog(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu b = j->tabla[sr][so];
    int irany     = (b.szin == FEHER) ? -1 : 1;
    int kezdo_sor = (b.szin == FEHER) ?  6 : 1;
    int ds        = cr - sr;
    int dob       = co - so;

    if (dob == 0) {
        if (ds == irany && j->tabla[cr][co].tipus == URES) return 1;
        if (sr == kezdo_sor && ds == 2 * irany &&
            j->tabla[sr + irany][so].tipus == URES &&
            j->tabla[cr][co].tipus == URES) return 1;
    }
    if (abs(dob) == 1 && ds == irany) {
        Babu cel = j->tabla[cr][co];
        if (cel.tipus != URES && cel.szin != b.szin) return 1;
    }
    return 0;
}

// B?stya
static int bastya(JatekAllapot *j, int sr, int so, int cr, int co) {
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

// Fut?
static int futo(JatekAllapot *j, int sr, int so, int cr, int co) {
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

// Husz?r
static int huszar(int sr, int so, int cr, int co) {
    int ds  = abs(cr - sr);
    int dob = abs(co - so);
    return (ds == 2 && dob == 1) || (ds == 1 && dob == 2);
}

// Kir?lyn? = b?stya + fut?
static int kiralyno(JatekAllapot *j, int sr, int so, int cr, int co) {
    return bastya(j, sr, so, cr, co) || futo(j, sr, so, cr, co);
}

// Kir?ly
static int kiraly(int sr, int so, int cr, int co) {
    return abs(cr - sr) <= 1 && abs(co - so) <= 1;
}

int babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!ervenyes(sr, so) || !ervenyes(cr, co)) return 0;
    Babu b = j->tabla[sr][so];
    if (b.tipus == URES) return 0;
    if (b.szin != j->kovetkezo) return 0;
    if (sr == cr && so == co) return 0;

    Babu cel = j->tabla[cr][co];
    if (cel.tipus != URES && cel.szin == b.szin) return 0;

    switch (b.tipus) {
        case GYALOG:   return gyalog(j, sr, so, cr, co);
        case BASTYA:   return bastya(j, sr, so, cr, co);
        case HUSZAR:   return huszar(sr, so, cr, co);
        case FUTO:     return futo(j, sr, so, cr, co);
        case KIRALYNO: return kiralyno(j, sr, so, cr, co);
        case KIRALY:   return kiraly(sr, so, cr, co);
        default:       return 0;
    }
}

int lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!babu_szabalyos_lepes(j, sr, so, cr, co)) return 0;

    Babu b = j->tabla[sr][so];

    // Gyalog?tv?ltoz?s: mindig kir?lyn?
    if (b.tipus == GYALOG && (cr == 0 || cr == 7))
        b.tipus = KIRALYNO;

    j->tabla[cr][co] = b;
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};

    const char oszlop_nev[8] = {'a','b','c','d','e','f','g','h'};
    snprintf(j->utolso_lepes, sizeof(j->utolso_lepes),
             "%c%d-%c%d", oszlop_nev[so], 8 - sr, oszlop_nev[co], 8 - cr);

    j->kovetkezo = ellenfel(j->kovetkezo);
    j->van_kivalasztva     = 0;
    j->kivalasztott_sor    = -1;
    j->kivalasztott_oszlop = -1;
    return 1;
}

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
