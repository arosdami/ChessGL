#include "rajz.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stb_image.h"

static GLFWwindow *ablak = NULL;
static unsigned int program;
static unsigned int VAO, VBO;
static int akt_szel = ABLAK_SZEL;
static int akt_mag  = ABLAK_MAG;

// ============================================================
//                    SHADER BETOLTES
// ============================================================
static unsigned int shader_betolt(const char *ut, GLenum tipus) {
    FILE *f = fopen(ut, "rb");
    if (!f) { printf("Nem talalhato: %s\n", ut); return 0; }
    fseek(f, 0, SEEK_END);
    long hossz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *tart = (char*)malloc(hossz + 1);
    fread(tart, 1, hossz, f);
    tart[hossz] = 0;
    fclose(f);

    unsigned int sh = glCreateShader(tipus);
    glShaderSource(sh, 1, (const char**)&tart, NULL);
    glCompileShader(sh);

    int ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(sh, 512, NULL, log);
        printf("Shader hiba (%s):\n%s\n", ut, log);
    }
    free(tart);
    return sh;
}

// ============================================================
//                    ABLAK IKON
// ============================================================
static void ablak_ikon_beallit(void) {
    int w, h, csatornak;
    unsigned char *pixels = stbi_load("resources/icon.png", &w, &h, &csatornak, 4);
    if (pixels) {
        GLFWimage ikon;
        ikon.width  = w;
        ikon.height = h;
        ikon.pixels = pixels;
        glfwSetWindowIcon(ablak, 1, &ikon);
        stbi_image_free(pixels);
        printf("Ablak ikon betoltve: resources/icon.png (%dx%d)\n", w, h);
    } else {
        printf("Figyelem: resources/icon.png nem talalhato, alap ikon hasznalva.\n");
    }
}

// ============================================================
//                    INICIALIZALAS
// ============================================================
int rajz_inicializalas(void) {
    if (!glfwInit()) return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    ablak = glfwCreateWindow(ABLAK_SZEL, ABLAK_MAG, "ChessGL by arosdami", NULL, NULL);
    if (!ablak) { glfwTerminate(); return 0; }

    glfwMakeContextCurrent(ablak);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 0;

    // Ablak ikon betoltese
    ablak_ikon_beallit();

    // Shaderek
    unsigned int vs = shader_betolt("shaders/vertex.glsl", GL_VERTEX_SHADER);
    unsigned int fs = shader_betolt("shaders/fragment.glsl", GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 32*3 csucs * 7 float elegendo (korhoz)
    glBufferData(GL_ARRAY_BUFFER, 8192 * 7 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    rajz_ablak_meret(ABLAK_SZEL, ABLAK_MAG);
    return 1;
}

void rajz_bezaras(void) {
    if (ablak) glfwDestroyWindow(ablak);
    glfwTerminate();
}

int rajz_ablak_bezarva(void) {
    return glfwWindowShouldClose(ablak);
}

void rajz_eszkozok_cserel(void) {
    glfwSwapBuffers(ablak);
    glfwPollEvents();
}

void rajz_torol(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void rajz_ablak_meret(int szel, int mag) {
    if (szel < 1) szel = 1;
    if (mag  < 1) mag  = 1;
    akt_szel = szel;
    akt_mag  = mag;
    glViewport(0, 0, szel, mag);

    glUseProgram(program);
    // Ortografikus: bal felul (0,0), jobb alul (szel, mag)
    float proj[16] = {
        2.0f / szel, 0,            0, 0,
        0,          -2.0f / mag,   0, 0,
        0,           0,           -1, 0,
       -1,           1,            0, 1
    };
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"),
                       1, GL_FALSE, proj);
}

// ============================================================
//                    RAJZOLO PRIMITIVEK
// ============================================================
static void csucsok_kuld(const float *adatok, int csucs_db) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    csucs_db * 7 * sizeof(float), adatok);
    glDrawArrays(GL_TRIANGLES, 0, csucs_db);
}

void rajz_negyzet(float x, float y, float w, float h,
                  float r, float g, float b) {
    float d[] = {
        x,     y,     0,0, r,g,b,
        x + w, y,     0,0, r,g,b,
        x + w, y + h, 0,0, r,g,b,
        x,     y,     0,0, r,g,b,
        x + w, y + h, 0,0, r,g,b,
        x,     y + h, 0,0, r,g,b,
    };
    csucsok_kuld(d, 6);
}

void rajz_negyzet_atlatszo(float x, float y, float w, float h,
                           float r, float g, float b, float a) {
    float rr = r * a;
    float gg = g * a;
    float bb = b * a;
    rajz_negyzet(x, y, w, h, rr, gg, bb);
}

void rajz_kor(float cx, float cy, float sugar,
              float r, float g, float b) {
    const int N = 32;
    float d[N * 3 * 7];
    int i = 0;
    for (int k = 0; k < N; k++) {
        float a1 = (float)k       / N * 6.2831853f;
        float a2 = (float)(k + 1) / N * 6.2831853f;

        d[i++] = cx; d[i++] = cy; d[i++] = 0; d[i++] = 0; d[i++] = r; d[i++] = g; d[i++] = b;
        d[i++] = cx + cosf(a1) * sugar; d[i++] = cy + sinf(a1) * sugar;
        d[i++] = 0; d[i++] = 0; d[i++] = r; d[i++] = g; d[i++] = b;
        d[i++] = cx + cosf(a2) * sugar; d[i++] = cy + sinf(a2) * sugar;
        d[i++] = 0; d[i++] = 0; d[i++] = r; d[i++] = g; d[i++] = b;
    }
    csucsok_kuld(d, N * 3);
}

void rajz_haromszog(float x1, float y1, float x2, float y2,
                    float x3, float y3,
                    float r, float g, float b) {
    float d[] = {
        x1, y1, 0,0, r,g,b,
        x2, y2, 0,0, r,g,b,
        x3, y3, 0,0, r,g,b,
    };
    csucsok_kuld(d, 3);
}

// ============================================================
//                    5x7 BITMAP FONT
// ============================================================
static const unsigned char FONT[96][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x00,0x00,0x5F,0x00,0x00}, // !
    {0x00,0x07,0x00,0x07,0x00}, // "
    {0x14,0x7F,0x14,0x7F,0x14}, // #
    {0x24,0x2A,0x7F,0x2A,0x12}, // $
    {0x23,0x13,0x08,0x64,0x62}, // %
    {0x36,0x49,0x55,0x22,0x50}, // &
    {0x00,0x05,0x03,0x00,0x00}, // '
    {0x00,0x1C,0x22,0x41,0x00}, // (
    {0x00,0x41,0x22,0x1C,0x00}, // )
    {0x14,0x08,0x3E,0x08,0x14}, // *
    {0x08,0x08,0x3E,0x08,0x08}, // +
    {0x00,0x50,0x30,0x00,0x00}, // ,
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x60,0x60,0x00,0x00}, // .
    {0x20,0x10,0x08,0x04,0x02}, // /
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x56,0x36,0x00,0x00}, // ;
    {0x08,0x14,0x22,0x41,0x00}, // <
    {0x14,0x14,0x14,0x14,0x14}, // =
    {0x00,0x41,0x22,0x14,0x08}, // >
    {0x02,0x01,0x51,0x09,0x06}, // ?
    {0x32,0x49,0x79,0x41,0x3E}, // @
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x3F,0x40,0x38,0x40,0x3F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x00,0x7F,0x41,0x41,0x00}, // [
    {0x02,0x04,0x08,0x10,0x20}, // backslash
    {0x00,0x41,0x41,0x7F,0x00}, // ]
    {0x04,0x02,0x01,0x02,0x04}, // ^
    {0x40,0x40,0x40,0x40,0x40}, // _
    {0x00,0x01,0x02,0x04,0x00}, // `
    {0x20,0x54,0x54,0x54,0x78}, // a
    {0x7F,0x48,0x44,0x44,0x38}, // b
    {0x38,0x44,0x44,0x44,0x20}, // c
    {0x38,0x44,0x44,0x48,0x7F}, // d
    {0x38,0x54,0x54,0x54,0x18}, // e
    {0x08,0x7E,0x09,0x01,0x02}, // f
    {0x0C,0x52,0x52,0x52,0x3E}, // g
    {0x7F,0x08,0x04,0x04,0x78}, // h
    {0x00,0x44,0x7D,0x40,0x00}, // i
    {0x20,0x40,0x44,0x3D,0x00}, // j
    {0x7F,0x10,0x28,0x44,0x00}, // k
    {0x00,0x41,0x7F,0x40,0x00}, // l
    {0x7C,0x04,0x18,0x04,0x78}, // m
    {0x7C,0x08,0x04,0x04,0x78}, // n
    {0x38,0x44,0x44,0x44,0x38}, // o
    {0x7C,0x14,0x14,0x14,0x08}, // p
    {0x08,0x14,0x14,0x18,0x7C}, // q
    {0x7C,0x08,0x04,0x04,0x08}, // r
    {0x48,0x54,0x54,0x54,0x20}, // s
    {0x04,0x3F,0x44,0x40,0x20}, // t
    {0x3C,0x40,0x40,0x20,0x7C}, // u
    {0x1C,0x20,0x40,0x20,0x1C}, // v
    {0x3C,0x40,0x30,0x40,0x3C}, // w
    {0x44,0x28,0x10,0x28,0x44}, // x
    {0x0C,0x50,0x50,0x50,0x3C}, // y
    {0x44,0x64,0x54,0x4C,0x44}, // z
    {0x00,0x08,0x36,0x41,0x00}, // {
    {0x00,0x00,0x7F,0x00,0x00}, // |
    {0x00,0x41,0x36,0x08,0x00}, // }
    {0x08,0x04,0x08,0x10,0x08}, // ~
    {0x00,0x00,0x00,0x00,0x00}, // (unused)
};

// Egy karakter kirajzolasa
static void karakter_rajzol(char c, float x, float y, float meret,
                            float r, float g, float b) {
    if (c < 32 || c > 126) c = '?';
    int idx = c - 32;
    float px = meret / 7.0f;

    for (int oszlop = 0; oszlop < 5; oszlop++) {
        unsigned char bajt = FONT[idx][oszlop];
        for (int sor = 0; sor < 7; sor++) {
            if (bajt & (1 << sor)) {
                rajz_negyzet(x + oszlop * px,
                             y + sor * px,
                             px, px, r, g, b);
            }
        }
    }
}

void rajz_szoveg(const char *szoveg, float x, float y,
                 float meret, float r, float g, float b) {
    float px = meret / 7.0f;
    float kurzor = x;
    while (*szoveg) {
        karakter_rajzol(*szoveg, kurzor, y, meret, r, g, b);
        kurzor += 6 * px;
        szoveg++;
    }
}

float rajz_szoveg_szelesseg(const char *szoveg, float meret) {
    float px = meret / 7.0f;
    return strlen(szoveg) * 6 * px;
}

// ============================================================
//                    EGER ES BILLENTYU
// ============================================================
int rajz_gomb_lenyomva(int gomb) {
    return glfwGetMouseButton(ablak, gomb) == GLFW_PRESS;
}

double rajz_eger_x(void) {
    double x, y;
    glfwGetCursorPos(ablak, &x, &y);
    return x;
}

double rajz_eger_y(void) {
    double x, y;
    glfwGetCursorPos(ablak, &x, &y);
    return y;
}

// ============================================================
//                    IDO ES TELJES KEPERNYO
// ============================================================
double rajz_ido(void) {
    return glfwGetTime();
}

static int teljes_kepernyos = 0;
static int elozo_x = 100, elozo_y = 100, elozo_w = ABLAK_SZEL, elozo_h = ABLAK_MAG;

void rajz_teljes_kepernyo(int be) {
    if (be == teljes_kepernyos) return;

    if (be) {
        glfwGetWindowPos(ablak, &elozo_x, &elozo_y);
        glfwGetWindowSize(ablak, &elozo_w, &elozo_h);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mod = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(ablak, monitor, 0, 0,
                             mod->width, mod->height, mod->refreshRate);
        teljes_kepernyos = 1;
    } else {
        glfwSetWindowMonitor(ablak, NULL, elozo_x, elozo_y,
                             elozo_w, elozo_h, 0);
        teljes_kepernyos = 0;
    }

    glfwPollEvents();

    int w, h;
    glfwGetFramebufferSize(ablak, &w, &h);
    rajz_ablak_meret(w, h);
}

int rajz_teljes_kepernyos(void) { return teljes_kepernyos; }

int rajz_ablak_szel(void) {
    int w, h;
    glfwGetFramebufferSize(ablak, &w, &h);
    return w;
}

int rajz_ablak_mag(void) {
    int w, h;
    glfwGetFramebufferSize(ablak, &w, &h);
    return h;
}