#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float colorR;
uniform float colorG;
uniform float colorB;

void main(){
    color = vec4(colorVS.x + colorR, colorVS.y + colorG, colorVS.z + colorB, 1.0);
}

