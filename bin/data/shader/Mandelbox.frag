#version 400
precision highp float;
in vec2 vTexCoord;
out vec4 outputColor;

uniform vec2 size;
uniform vec3 cp;

uniform float minRadius2;
uniform float fixedRadius2;
uniform float foldingLimit;
uniform float scale;
uniform float rep;
uniform vec3 offset;

float pixel_size;

void sphereFold(inout vec3 z, inout float dz){
    float r2 = dot(z, z);
    if (r2 < minRadius2) {
        float tmp = fixedRadius2 / minRadius2;
        z *= tmp;
        dz *= tmp;
    } else if (r2 < fixedRadius2) {
        float tmp = fixedRadius2 / r2;
        z *= tmp;
        dz *= tmp;
    }
}

void boxFold(inout vec3 z, inout float dz){
    z = clamp(z, - foldingLimit, foldingLimit) * 2.0 - z;
}

float DF(vec3 z){
    //z.xy = mod(z.xy + vec2(rep * 0.5), rep) - rep * 0.5;
    float dr = 1.0;

    for (int n = 0; n < 12; n++) {

        boxFold(z, dr);
        sphereFold(z, dr);

        z = scale * z + offset;
        dr = dr * abs(scale) + 1.0;
    }
    return length(z) / abs(dr);
}

#pragma include "render.frag"
