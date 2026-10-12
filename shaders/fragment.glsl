#version 330 core
in vec2 TexCoord;
in vec3 Szin;
out vec4 FragColor;

uniform sampler2D textura;
uniform int hasznal_textura;

void main() {
    if (hasznal_textura == 1) {
        vec4 t = texture(textura, TexCoord);
        if (t.a < 0.1) discard;
        FragColor = t;
    } else {
        FragColor = vec4(Szin, 1.0);
    }
}
