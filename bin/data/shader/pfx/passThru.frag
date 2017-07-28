#version 410

uniform sampler2DRect tex0;
uniform float opacity;

in vec4 vColor;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {

    vec4 col = texture(tex0, vTexCoord);
    col.rgb = col.rgb * (1. - opacity);

    outputColor = col;
}
