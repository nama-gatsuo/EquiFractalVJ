void main(){
    vec2 q = vTexCoord / size;
    vec2 uv = 2.0 * q - 1.0;
    uv.x *= size.x / size.y;

    vec3 ro = cp;
    vec3 cf = normalize(vec3(0.0) - ro);
    vec3 cs = normalize(cross(cf, vec3(0.0, 1.0, 1.0)));
    vec3 cu = normalize(cross(cs, cf));
    float depth = 2.8;
    vec3 rd = normalize(cs * uv.x + cu * uv.y + cf * depth);

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
    vec4 col = vec4(vec3(c) + vec3(0.3, 0.3, 0.2) * cnt * 0.04, 1.0);
    col.xyz = vec3(0.95) - col.xyz;
    //col.xyz = pow(col.xyz, vec3(1./2.2));
    outputColor = col;
}
