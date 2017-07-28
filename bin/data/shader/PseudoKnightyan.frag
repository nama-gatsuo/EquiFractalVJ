#version 400
precision highp float;
in vec2 vTexCoord;
out vec4 outputColor;

uniform vec2 size;
uniform vec3 cp;
uniform vec3 uCol;

uniform float kFactor;
uniform vec3 cSize;
uniform float offset;

float DF(vec3 p){

	float DEfactor=1.;
	for(int i = 0; i < 6; i++){
		p = 2. * clamp(p, -cSize, cSize) - p;
		float k = max(kFactor / dot(p, p), 1.);
		p *= k;
		DEfactor *= k * 1.1;
	}
	float rxy=length(p.xy);
	return max(rxy - offset, abs(rxy * p.z) / length(p)) / DEfactor;

}

#pragma include "EquiRenderer.frag"
