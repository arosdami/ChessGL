#ifndef RAJZ_H
#define RAJZ_H

#include "tabla.h"

#define ABLAK_SZEL 1000
#define ABLAK_MAG  760
#define RAJZ_MIN_MERET 1
#define RAJZ_EGY_DB 1
#define RAJZ_KETTO 2.0f
#define RAJZ_SHADER_LOG_MERET 512
#define RAJZ_RGBA_CSATORNAK 4
#define RAJZ_VERTEX_ADAT 7
#define RAJZ_MAX_CSUCSSZAM 8192
#define RAJZ_MATRIX_ELEMEK 16
#define RAJZ_POZICIO_ATTR 0
#define RAJZ_TEXTURA_ATTR 1
#define RAJZ_SZIN_ATTR 2
#define RAJZ_POZICIO_ADAT 2
#define RAJZ_TEXTURA_ADAT 2
#define RAJZ_SZIN_ADAT 3
#define RAJZ_TEXTURA_OFFSET 2
#define RAJZ_SZIN_OFFSET 4
#define RAJZ_NEGYZET_CSUCSSZAM 6
#define RAJZ_HAROMSZOG_CSUCSSZAM 3
#define RAJZ_KOR_SZELETEK 32
#define RAJZ_TELJES_KOR 6.2831853f
#define BETU_OSZLOPOK 5
#define BETU_SOROK 7
#define BETU_ASCII_KEZDO 32
#define BETU_ASCII_VEGE 126
#define BETU_KOZ 6

int  rajz_inicializalas(void);
void rajz_bezaras(void);
void rajz_ablak_meret(int szel, int mag);
int  rajz_ablak_bezarva(void);
void rajz_eszkozok_cserel(void);
void rajz_torol(float r, float g, float b);

void rajz_negyzet(float x, float y, float w, float h,
                  float r, float g, float b);
void rajz_negyzet_atlatszo(float x, float y, float w, float h,
                           float r, float g, float b, float a);
void rajz_kor(float cx, float cy, float sugar,
              float r, float g, float b);
void rajz_haromszog(float x1, float y1, float x2, float y2,
                    float x3, float y3,
                    float r, float g, float b);

void rajz_szoveg(const char *szoveg, float x, float y,
                 float meret, float r, float g, float b);
float rajz_szoveg_szelesseg(const char *szoveg, float meret);

int  rajz_gomb_lenyomva(int gomb);
double rajz_eger_x(void);
double rajz_eger_y(void);

double rajz_ido(void);
void   rajz_teljes_kepernyo(int be);
int    rajz_teljes_kepernyos(void);
int    rajz_ablak_szel(void);
int    rajz_ablak_mag(void);

/* Texturazott negyszog. */
void rajz_textura_negyszog(unsigned int textura, float x, float y,
                           float w, float h,
                           float u0, float v0, float u1, float v1);

/* Textura kor alakban. */
void rajz_textura_kor(unsigned int textura, float cx, float cy, float sugar);

/* Textura betoltese fajlbol. */
unsigned int rajz_textura_betolt(const char *ut);

#endif
