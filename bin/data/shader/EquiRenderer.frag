const float PI = 3.14159265359;

void main(){
    vec2 st = vTexCoord / size;
    float lon = st.x * PI * 2.;
    float lat = st.y * PI;

    vec3 ro = cp;
    vec3 rd = vec3(cos(lon) * sin(lat), cos(lat), sin(lon) * sin(lat));

    float d = 1., td = 1.;
    const float md = 100.;
    float cnt = 0.;

    for (int i = 0; i < 100; i++) {
        d = DF(ro + td * rd);
        td += d;
        cnt += 1.0;
        if (d < 0.001) break;
        if (td > md) break;
    }

    float c = td * 0.01;
    vec4 col = vec4(vec3(c) + vec3(0.03, 0.03, 0.02) * cnt*0.4, 1.0);
    col.xyz = vec3(1.0) - col.xyz;
    //col.xyz = pow(col.xyz, vec3(1./2.2));
    outputColor = col;
}
