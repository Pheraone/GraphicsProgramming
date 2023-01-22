#version 330 core

#define MAXSTEPS 1000
#define MINDIST .01
out vec4 fragColor;
in vec2 fragCoord;
uniform float iTime;
uniform vec2 iResolution;


//sdf - signed distance functions -> sdShape 
float sdShape(vec3 p) {
    p.z += iTime;
    return length(
        cos(p) - .05 * cos(2. * p.y * p.x) + .1 * sin(3. * (.1 * p.x - p.y + p.z))) - 1.;

}

float sdBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}



float box(vec3 pos) {
    pos = pos + 2. * vec3(2. * iTime, 0 , 6. * iTime);
    //scale
    pos *= .1;
    pos.xy *= 1;
    pos.y -= 1;
    
    float base = sdBox(mod(pos, 0.8), vec3(.4, .4, .4)) / 1.5;
    return base;
} 



float DistanceEstimator(vec3 pos) {
    
   
    float aap = box(pos);
    float base = sdBox(mod(pos, 0.8), vec3(.4, .4, .4)) / 1.5;
    //old spheres
    //float d1 = distance(mod(pos, 1.), vec3(1, 1, 1)) - -1;

    return aap;
}

float trace(vec3 from, vec3 direction) {
    float totalDistance = .0;
    int steps;
    //raymarch
    for (steps = 0; steps < MAXSTEPS; steps++) {
        vec3 p = from + totalDistance * direction;
        //add shape
        p *= sdShape(p) - 1;
        float dist = DistanceEstimator(p);
        totalDistance += dist;
        if (dist < MINDIST) break;
    }
    return 1.0 - float(steps) / float(MAXSTEPS);
}

void main() {

    vec2 uv = (fragCoord - .5 * iResolution.xy) / iResolution.y;
    float depth = .0;
    depth = min(6., depth);
    vec3 camPos = vec3( 6, 0, 5.);
    vec3 camViewDir = normalize(vec3(uv.xy, 1));

    float dist = trace(camPos, camViewDir);
    
    vec3 color = (abs(sdShape(camPos - dist)) * 0.5 + 0.5 * -sin(( dist * 3.) + camPos * 2. + vec3(1, 4, 3))) * (2 + dist * 6);
    color *= exp(depth);
    fragColor = vec4(color , .5);
}

