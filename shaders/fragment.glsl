#version 330 core
in vec3 Szin;
out vec4 FragColor;

void main() {
    FragColor = vec4(Szin, 1.0);
}
