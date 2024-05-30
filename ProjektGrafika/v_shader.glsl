#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Atrybuty
in vec4 vertex;
in vec4 normal;
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 lightVector;
out vec4 normalVector;
out vec4 vVector;
out vec2 iTexCoord0;

void main(void) {
    lightVector = normalize(V * (lp - M * vertex));
    normalVector = normalize(V * M * normal);
    vVector = normalize(V[0] - V * M * vertex);

    iTexCoord0 = texCoord0;

    gl_Position = P * V * M * vertex;
}
