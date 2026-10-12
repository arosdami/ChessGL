#ifndef TABLA_H
#define TABLA_H

#define MERET 8

// B?but?pusok
typedef enum {
    URES = 0,
    GYALOG,
    BASTYA,
    HUSZAR,
    FUTO,
    KIRALYNO,
    KIRALY
} BabuTipus;

// Sz?nek
typedef enum {
    NINCS_SZIN = 0,
    FEHER,
    FEKETE
} Szin;

// Egy b?bu a t?bl?n
typedef struct {
    BabuTipus tipus;
    Szin      szin;
} Babu;

// A j?t?k teljes ?llapota
typedef struct {
    Babu  tabla[MERET][MERET];
    Szin  kovetkezo;
    int   kivalasztott_sor;
    int   kivalasztott_oszlop;
    int   van_kivalasztva;
    int   jatek_vege;              // 0=folyamatban, 1=matt, 2=patt
    char  utolso_lepes[24];
} JatekAllapot;

void tabla_inicializalas(JatekAllapot *j);
int  babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co);
int  lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co);
void lepesek_listazasa(JatekAllapot *j, int sor, int oszlop, int tomb[MERET][MERET]);
int  van_legalis_lepes(JatekAllapot *j, Szin szin);

#endif
