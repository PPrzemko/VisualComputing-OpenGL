#version 330 core

in vec3 colorVS;

//fragment shader output
out vec4 color;

uniform float colorAnim;

void main(){
    color = vec4(colorVS.x + colorAnim, colorVS.y - colorAnim, colorVS.z + colorAnim, 1.0);
}

