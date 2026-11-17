#version 330 core
const int TEXTURA_KIKAPCSOLVA = 0;
const int TEXTURA_BEKAPCSOLVA = 1;
const float UV_KOZEP = 0.5;
const float KOR_MASZK_SUGAR_NEGYZET = 0.25;
const float ALFA_MINIMUM = 0.02;

in vec2 TexCoord;
in vec3 Szin;
out vec4 FragColor;

uniform sampler2D textura;
uniform int hasznal_textura;
uniform int kormaszk;

void main() {
    if (hasznal_textura == TEXTURA_BEKAPCSOLVA) {
        // Eloszor a kor-maszk: ami a koron kivul esik, eldobjuk
        if (kormaszk == TEXTURA_BEKAPCSOLVA) {
            vec2 kp = TexCoord - vec2(UV_KOZEP, UV_KOZEP);
            if (dot(kp, kp) > KOR_MASZK_SUGAR_NEGYZET) discard;
        }

        vec4 t = texture(textura, TexCoord);
        if (t.a < ALFA_MINIMUM) discard;

        FragColor = t;
    } else {
        FragColor = vec4(Szin, 1.0);
    }
}
