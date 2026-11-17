#version 330 core
#define POZICIO_ATTRIBUTUM 0
#define TEXTURA_ATTRIBUTUM 1
#define SZIN_ATTRIBUTUM 2
const float Z_SIK = 0.0;
const float HOMOGEN_KOORDINATA = 1.0;

layout (location = POZICIO_ATTRIBUTUM) in vec2 aPos;
layout (location = TEXTURA_ATTRIBUTUM) in vec2 aTex;
layout (location = SZIN_ATTRIBUTUM) in vec3 aColor;

uniform mat4 projection;

out vec2 TexCoord;
out vec3 Szin;

void main() {
    gl_Position = projection * vec4(aPos, Z_SIK, HOMOGEN_KOORDINATA);
    TexCoord = aTex;
    Szin = aColor;
}
