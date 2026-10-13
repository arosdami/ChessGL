#include "tabla.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ============================================================
//                    SEGEDFUGGVENYEK
// ============================================================
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

// ============================================================
//                    TABLA INICIALIZALAS
// ============================================================
void tabla_inicializalas(JatekAllapot *j) {
    for (int s = 0; s < MERET; s++)
        for (int o = 0; o < MERET; o++)
            j->tabla[s][o] = (Babu){URES, NINCS_SZIN};

    BabuTipus hatso[MERET] = {
        BASTYA, HUSZAR, FUTO, KIRALYNO, KIRALY, FUTO, HUSZAR, BASTYA
    };

    for (int o = 0; o < MERET; o++) {
        j->tabla[0][o] = (Babu){hatso[o], FEKETE};
        j->tabla[1][o] = (Babu){GYALOG,  FEKETE};
        j->tabla[6][o] = (Babu){GYALOG,  FEHER};
        j->tabla[7][o] = (Babu){hatso[o], FEHER};
    }

    j->kovetkezo = FEHER;
    j->kivalasztott_sor   = -1;
    j->kivalasztott_oszlop= -1;
    j->van_kivalasztva    = 0;
    j->jatek_vege         = 0;
    strcpy(j->utolso_lepes, "-");

    j->feher_kiraly_mozdult    = 0;
    j->fekete_kiraly_mozdult   = 0;
    j->feher_bastya_a_mozdult  = 0;
    j->feher_bastya_h_mozdult  = 0;
    j->fekete_bastya_a_mozdult = 0;
    j->fekete_bastya_h_mozdult = 0;

    j->en_passant_sor    = -1;
    j->en_passant_oszlop = -1;
}

// ============================================================
//                    GYALOG
// ============================================================
static int gyalog_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu b = j->tabla[sr][so];
    int irany     = (b.szin == FEHER) ? -1 : 1;
    int kezdo_sor = (b.szin == FEHER) ?  6 : 1;
    int ds        = cr - sr;
    int dob       = co - so;

    // Egyenes el?re
    if (dob == 0) {
        if (ds == irany && ures_mezot_ellenoriz(j, cr, co)) return 1;
        if (sr == kezdo_sor && ds == 2 * irany &&
            ures_mezot_ellenoriz(j, sr + irany, so) &&
            ures_mezot_ellenoriz(j, cr, co)) return 1;
        return 0;
    }

    // ?tl?s ?t?s
    if (abs(dob) == 1 && ds == irany) {
        // Norm?l ?t?s
        if (ellenfel_babu_e(j, cr, co, b.szin)) return 1;

        // En passant
        if (j->en_passant_sor == cr && j->en_passant_oszlop == co) {
            // Az en passant mez? m?g?tt kell lennie az ellenf?l gyalogj?nak
            return 1;
        }
    }
    return 0;
}

// ============================================================
//                    BASTYA
// ============================================================
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

// ============================================================
//                    FUT?
// ============================================================
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

// ============================================================
//                    HUSZ?R (L-ALAK)
// ============================================================
static int huszar_szabalyos(int sr, int so, int cr, int co) {
    int ds  = abs(cr - sr);
    int dob = abs(co - so);
    // L-alak: 2+1 vagy 1+2
    return (ds == 2 && dob == 1) || (ds == 1 && dob == 2);
}

// ============================================================
//                    KIR?LYN?
// ============================================================
static int kiralyno_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    return bastya_szabalyos(j, sr, so, cr, co) ||
           futo_szabalyos(j, sr, so, cr, co);
}

// ============================================================
//                    KIR?LY
// ============================================================
static int kiraly_szabalyos(JatekAllapot *j, int sr, int so, int cr, int co) {
    Babu b = j->tabla[sr][so];

    // Egyszer? kir?ly l?p?s: 1 mez? b?rmely ir?nyba
    if (abs(cr - sr) <= 1 && abs(co - so) <= 1 &&
        !(sr == cr && so == co)) {
        // Az ?j mez?n nem lehet saj?t b?bu
        if (sajat_babu_e(j, cr, co, b.szin)) return 0;
        return 1;
    }

    // S?ncol?s (castling)
    if (sr == cr && abs(co - so) == 2) {
        // Kir?ly nem mozdult m?g el
        if (b.szin == FEHER && j->feher_kiraly_mozdult) return 0;
        if (b.szin == FEKETE && j->fekete_kiraly_mozdult) return 0;

        // R?vid s?ncol?s (kir?ly oldal): e->g (oszlop 4->6)
        if (co == 6) {
            int bastya_oszlop = 7;
            int bastya_mozdult = (b.szin == FEHER)
                                 ? j->feher_bastya_h_mozdult
                                 : j->fekete_bastya_h_mozdult;
            if (bastya_mozdult) return 0;
            // Ellen?rizz?k, hogy ?resek a k?ztes mez?k
            if (!ures_mezot_ellenoriz(j, sr, 5)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, 6)) return 0;
            // Ellen?rizz?k, hogy a b?stya ott van
            if (j->tabla[sr][bastya_oszlop].tipus != BASTYA) return 0;
            if (j->tabla[sr][bastya_oszlop].szin != b.szin) return 0;
            return 1;
        }

        // Hossz? s?ncol?s (kir?lyn? oldal): e->c (oszlop 4->2)
        if (co == 2) {
            int bastya_oszlop = 0;
            int bastya_mozdult = (b.szin == FEHER)
                                 ? j->feher_bastya_a_mozdult
                                 : j->fekete_bastya_a_mozdult;
            if (bastya_mozdult) return 0;
            if (!ures_mezot_ellenoriz(j, sr, 1)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, 2)) return 0;
            if (!ures_mezot_ellenoriz(j, sr, 3)) return 0;
            if (j->tabla[sr][bastya_oszlop].tipus != BASTYA) return 0;
            if (j->tabla[sr][bastya_oszlop].szin != b.szin) return 0;
            return 1;
        }
    }

    return 0;
}

// ============================================================
//                    SAKK ELLEN?RZ?S
// ============================================================
// Visszaadja, hogy az adott mez?t t?madja-e az ellenf?l
// (a sajat_kizaras sz?n kiv?tel?vel)
static int mezot_tamadja(JatekAllapot *j, int cel_sor, int cel_oszlop,
                         Szin tamado, int sajat_kizaras) {
    for (int s = 0; s < MERET; s++) {
        for (int o = 0; o < MERET; o++) {
            Babu b = j->tabla[s][o];
            if (b.tipus == URES || b.szin != tamado) continue;
            if (sajat_kizaras && s == cel_sor && o == cel_oszlop) continue;

            // Ideiglenesen ?t?ll?tjuk a k?vetkez?t, hogy a
            // babu_szabalyos_lepes ne ellen?rizze a sz?nt
            Szin mentett = j->kovetkezo;
            j->kovetkezo = tamado;

            int tamad = 0;
            int ds = cel_sor - s, dob = cel_oszlop - o;

            switch (b.tipus) {
                case GYALOG:
                    // Gyalog csak ?tl?san ?t
                    if (abs(dob) == 1) {
                        int irany = (b.szin == FEHER) ? -1 : 1;
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

// Megkeresi a kir?lyt
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

// ============================================================
//                    ?LTAL?NOS L?P?S-ELLEN?RZ?S
// ============================================================
int babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!ervenyes(sr, so) || !ervenyes(cr, co)) return 0;
    Babu b = j->tabla[sr][so];
    if (b.tipus == URES) return 0;
    if (b.szin != j->kovetkezo) return 0;
    if (sr == cr && so == co) return 0;

    // C?lmez?n saj?t b?bu?
    if (sajat_babu_e(j, cr, co, b.szin)) return 0;

    // B?bu-specifikus szab?ly
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

    // Kir?ly nem maradhat sakkban
    // Szimul?ljuk a l?p?st
    Babu le = j->tabla[cr][co];
    Babu mozgo = j->tabla[sr][so];
    j->tabla[cr][co] = mozgo;
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};

    int sajat_sakk = sakkban_van_e(j, b.szin);

    // Vissza?ll?t?s
    j->tabla[sr][so] = mozgo;
    j->tabla[cr][co] = le;

    if (sajat_sakk) return 0;
    return 1;
}

// ============================================================
//                    L?P?S V?GREHAJT?S
// ============================================================
int lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co) {
    if (!babu_szabalyos_lepes(j, sr, so, cr, co)) return 0;

    Babu b = j->tabla[sr][so];

    // En passant: ha gyalog ?tl?san ?res mez?re l?p, akkor az
    // "en passant" szab?ly szerint ?t
    int en_passant = 0;
    if (b.tipus == GYALOG && abs(co - so) == 1 &&
        j->tabla[cr][co].tipus == URES) {
        // Az en passant mez? m?g?tt lev? gyalogot levessz?k
        j->tabla[sr][co] = (Babu){URES, NINCS_SZIN};
        en_passant = 1;
    }

    // S?ncol?s: a kir?ly 2 mez?t l?p
    int sancolas = 0;
    if (b.tipus == KIRALY && abs(co - so) == 2) {
        sancolas = 1;
        if (co == 6) {
            // R?vid s?ncol?s: b?stya a sarokb?l az f oszlopba
            j->tabla[sr][5] = j->tabla[sr][7];
            j->tabla[sr][7] = (Babu){URES, NINCS_SZIN};
        } else if (co == 2) {
            // Hossz? s?ncol?s: b?stya a sarokb?l a d oszlopba
            j->tabla[sr][3] = j->tabla[sr][0];
            j->tabla[sr][0] = (Babu){URES, NINCS_SZIN};
        }
    }

    // Gyalog?tv?ltoz?s: mindig kir?lyn?
    if (b.tipus == GYALOG && (cr == 0 || cr == 7)) {
        b.tipus = KIRALYNO;
    }

    // Kir?ly/b?stya mozdult-e
    if (b.tipus == KIRALY) {
        if (b.szin == FEHER) j->feher_kiraly_mozdult = 1;
        else                 j->fekete_kiraly_mozdult = 1;
    }
    if (b.tipus == BASTYA) {
        if (b.szin == FEHER && so == 0) j->feher_bastya_a_mozdult = 1;
        if (b.szin == FEHER && so == 7) j->feher_bastya_h_mozdult = 1;
        if (b.szin == FEKETE && so == 0) j->fekete_bastya_a_mozdult = 1;
        if (b.szin == FEKETE && so == 7) j->fekete_bastya_h_mozdult = 1;
    }

    // L?p?s v?grehajt?sa
    j->tabla[cr][co] = b;
    j->tabla[sr][so] = (Babu){URES, NINCS_SZIN};

    // En passant lehet?s?g be?ll?t?sa a k?vetkez? l?p?sre
    j->en_passant_sor    = -1;
    j->en_passant_oszlop = -1;
    if (b.tipus == GYALOG && abs(cr - sr) == 2) {
        // Dupla gyalogl?p?s: az "?tugrott" mez?re en passant lehet
        j->en_passant_sor    = (sr + cr) / 2;
        j->en_passant_oszlop = so;
    }

    // L?p?s sz?veges form?ban
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
    if (sancolas)   specialis = (co == 6) ? 'r' : 'l';

    snprintf(j->utolso_lepes, sizeof(j->utolso_lepes),
             "%c%c%c%c-%c%c%c",
             szin_jel, tipus_jel, oszlop_nev[so], '8' - sr,
             oszlop_nev[co], '8' - cr, specialis);

    // K?vetkez? j?t?kos
    j->kovetkezo = ellenfel(j->kovetkezo);

    // J?t?k v?ge ellen?rz?s
    if (!van_legalis_lepes(j, j->kovetkezo)) {
        if (sakkban_van_e(j, j->kovetkezo)) {
            j->jatek_vege = 1;  // Matt
        } else {
            j->jatek_vege = 2;  // Patt
        }
    }

    j->van_kivalasztva     = 0;
    j->kivalasztott_sor    = -1;
    j->kivalasztott_oszlop = -1;
    return 1;
}

// ============================================================
//                    L?P?SEK LIST?Z?SA
// ============================================================
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
