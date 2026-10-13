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
    char  utolso_lepes[32];

    // S?ncol?shoz: melyik kir?ly/b?stya mozdult m?r el
    int   feher_kiraly_mozdult;
    int   fekete_kiraly_mozdult;
    int   feher_bastya_a_mozdult;
    int   feher_bastya_h_mozdult;
    int   fekete_bastya_a_mozdult;
    int   fekete_bastya_h_mozdult;

    // En passant: az utols? l?p?s dupla gyalogl?p?s volt-e,
    // ?s melyik oszlopba ?rt
    int   en_passant_sor;    // -1, ha nincs
    int   en_passant_oszlop;
} JatekAllapot;

// Alap f?ggv?nyek
void tabla_inicializalas(JatekAllapot *j);
int  babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co);
int  lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co);
void lepesek_listazasa(JatekAllapot *j, int sor, int oszlop, int tomb[MERET][MERET]);
int  van_legalis_lepes(JatekAllapot *j, Szin szin);
int  sakkban_van_e(JatekAllapot *j, Szin szin);

#endif
