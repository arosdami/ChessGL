#ifndef TABLA_H
#define TABLA_H

#define MERET 8
#define TABLA_SOR_FEKETE_FIGURA 0
#define TABLA_SOR_FEKETE_GYALOG 1
#define TABLA_SOR_FEHER_GYALOG 6
#define TABLA_SOR_FEHER_FIGURA 7
#define TABLA_NINCS_MEZO -1
#define UTOLSO_LEPES_HOSSZ 32
#define JATEK_FOLYAMATBAN 0
#define JATEK_MATT 1
#define JATEK_PATT 2
#define TABLA_EGY_MEZO 1
#define TABLA_KET_MEZO 2
#define TABLA_HAROM_MEZO 3

/* A tabla elemein hasznalt babutipusok. */
typedef enum {
    URES = JATEK_FOLYAMATBAN,
    GYALOG,
    BASTYA,
    HUSZAR,
    FUTO,
    KIRALYNO,
    KIRALY
} BabuTipus;

/* A jatekos szinei. */
typedef enum {
    NINCS_SZIN = JATEK_FOLYAMATBAN,
    FEHER,
    FEKETE
} Szin;

/* Egy tabla-mezo tartalma. */
typedef struct {
    BabuTipus tipus;
    Szin      szin;
} Babu;

/* A jatek teljes allapota. */
typedef struct {
    Babu  tabla[MERET][MERET];
    Szin  kovetkezo;
    int   kivalasztott_sor;
    int   kivalasztott_oszlop;
    int   van_kivalasztva;
    int   jatek_vege;
    char  utolso_lepes[UTOLSO_LEPES_HOSSZ];

    /* A sancolas engedelyezesehez szukseges allapot. */
    int   feher_kiraly_mozdult;
    int   fekete_kiraly_mozdult;
    int   feher_bastya_a_mozdult;
    int   feher_bastya_h_mozdult;
    int   fekete_bastya_a_mozdult;
    int   fekete_bastya_h_mozdult;

    /* Az utolso dupla gyaloglepes utani en passant celmezo. */
    int   en_passant_sor;
    int   en_passant_oszlop;
} JatekAllapot;

/* A tabla publikus muveletei. */
void tabla_inicializalas(JatekAllapot *j);
int  babu_szabalyos_lepes(JatekAllapot *j, int sr, int so, int cr, int co);
int  lepes_vegrehajtas(JatekAllapot *j, int sr, int so, int cr, int co);
void lepesek_listazasa(JatekAllapot *j, int sor, int oszlop, int tomb[MERET][MERET]);
int  van_legalis_lepes(JatekAllapot *j, Szin szin);
int  sakkban_van_e(JatekAllapot *j, Szin szin);

#endif
