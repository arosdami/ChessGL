#version 330 core
in vec2 TexCoord;
in vec3 Szin;
out vec4 FragColor;

uniform sampler2D textura;
uniform int hasznal_textura;
uniform int kormaszk;

void main() {
    if (hasznal_textura == 1) {
        // Eloszor a kor-maszk: ami a koron kivul esik, eldobjuk
        if (kormaszk == 1) {
            vec2 kp = TexCoord - vec2(0.5, 0.5);
            if (dot(kp, kp) > 0.25) discard;
        }

        vec4 t = texture(textura, TexCoord);
        if (t.a < 0.02) discard;

        FragColor = t;
    } else {
        FragColor = vec4(Szin, 1.0);
    }
}
