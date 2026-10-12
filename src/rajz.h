#ifndef RAJZ_H
#define RAJZ_H

#include "tabla.h"

#define ABLAK_SZEL 1000
#define ABLAK_MAG  760

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
// Texturazott negyszog
void rajz_textura_negyszog(unsigned int textura, float x, float y,
                           float w, float h,
                           float u0, float v0, float u1, float v1);

// Textura betoltese fajlbol
unsigned int rajz_textura_betolt(const char *ut);

#endif
