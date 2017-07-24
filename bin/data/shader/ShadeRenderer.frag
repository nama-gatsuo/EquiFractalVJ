float pixel_size;

vec2 intersect(in vec3 ro, in vec3 rd){
    float t = 1.0;
    float d = 1.0;
    float tm = 0.0;
    float dm = 100.0;
    float pd = 100.0;
    float os = 0.0;
    float s = 0.0;

    for (int i = 0; i < 100; i++) {

        if (d < pixel_size * t || t > 40.0){
        } else {
            d = DF(ro + t * rd);

            if (d > os) {
                os = 0.8 * d * d / pd;
                s = d + os;
                pd = d;
            } else {
                s = - os;
                os = 0.0;
                pd = 100.0;
                d = 1.0;
            }

            if (d < dm * t) {
                tm = t;
                dm = d;
            }

            t += s;
        }

    }
    return vec2(tm, dm);
}

float softshadow(vec3 ro, vec3 rd, float k){
    float akuma = 1.0, h = 0.0;
    float t = 0.01;
    for (int i = 0; i < 50; i++){
        h = DF(ro + rd * t);
        if (h < 0.001) return 0.02;
        akuma = min(akuma, k * h / t);
        t += clamp(h, 0.01, 2.0);
    }
    return akuma;
}

vec3 lighting(vec3 p, vec3 rd, float ps){
    vec3 l1_dir = normalize(vec3(0.8, 0.8, 0.4));
    vec3 l1_col = vec3(1.37, 0.99, 0.79);
    vec3 l2_dir = normalize(vec3(-0.8, 0.5, 0.3));
    vec3 l2_col = vec3(0.89, 0.99, 1.3);

    vec3 e = vec3(0.5 * ps,0.0,0.0);
 	vec3 n = normalize(vec3(
        DF(p+e.xyy)-DF(p-e.xyy),
 		DF(p+e.yxy)-DF(p-e.yxy),
 		DF(p+e.yyx)-DF(p-e.yyx)
    ));

    float shadow = softshadow(p, l1_dir, 10.0);

    float dif1 = max(0.3, dot(n, l1_col));
    float dif2 = max(0.3, dot(n, l2_col));
    float bac1 = max(0.2 + 0.8 * dot(vec3(-l1_dir.x, -1.0, -l1_dir.z), n), 0.0);
    float bac2 = max(0.2 + 0.8 * dot(vec3(-l2_dir.x, -1.0, -l2_dir.z), n), 0.0);
    float spe = max(0.0, pow(clamp(dot(l1_dir, reflect(rd, n)), 0.0, 1.0), 10.0));

    vec3 col = 1.5 * l1_col * dif1 * shadow;
    col += 1.1 * l2_col * dif2;
    col += 0.3 * bac1 * l1_col;
    col += 0.3 * bac2 * l2_col;
    col += 4.0 * spe;

    return col;
}

vec3 post(vec3 col, vec2 q){
    col = pow(clamp(col, 0.0, 1.0), vec3(0.55));
    col = col * 0.6 + 0.4 * col * col * (3.0 - 2.0 * col);
    col = mix(col, vec3(dot(col, vec3(0.33))), -0.5);
    col *= 0.5 + 0.5 * pow(19.0 * q.x * q.y * (1.0 - q.x) * (1.0 - q.y), 0.7);
    return col;
}

void main(){
    vec2 q = vTexCoord / size;
    vec2 uv = 2.0 * q - 1.0;
    uv.x *= size.x / size.y;

    pixel_size = 1.0 / (size.y * 2.8);

    vec3 ro = cp;
    vec3 cf = normalize(vec3(0.0) - ro);
    vec3 cs = normalize(cross(cf, vec3(0.0, 1.0, 1.0)));
    vec3 cu = normalize(cross(cs, cf));
    float depth = 2.8;
    vec3 rd = normalize(cs * uv.x + cu * uv.y + cf * depth);
    vec3 p = ro;

    vec2 result = intersect(ro, rd);
    vec3 col = vec3(1.);
    float t = result.x, d = result.y;

    if (d < pixel_size * t) {
        p = ro + t * rd;
        col = lighting(p, rd, pixel_size * t) * vec3(1.0, 1.1, 1.3) * 0.2;
        col = mix(col, vec3(0.0), 1.0 - exp(-0.001*t*t));
    }
    outputColor = vec4(post(col, q), 1.0);
}
