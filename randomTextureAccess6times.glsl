// for shadertoy
// with a 1024x1024 texture, and 1092x1080 resolution, this was doing over 50fps, which is good!
// implicit inputs:
//      void main() {  mainImage(fragColor, gl_FragCoord.xy); }
//      uniform vec2 iResolution;
//      uniform float iTime;
//      uniform sampler2D iChannel0;

vec2 getScaleFactor(float time, int i)
{
    float fi = float(i);
    float ph = mod(time, 10.0);
    float t = mod(time, 5.0);
    vec2 fun = vec2(sin(fi), cos(fi));
    if(mod(time, 20.0) > 10.0)
      fun = vec2(fi, fi);
    if(ph < 5.0)
        return t / 10.0 * fun;
       else
        return (5.0 - t) / 10.0 * fun;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;
    vec3 col = vec3(0, 0, 0);
    float weight = 1.0/6.0;
    for(int i = 0; i < 6; ++i) {
        vec2 luv = uv + getScaleFactor(iTime, i);
        luv = vec2(fract(luv.s), fract(luv.t));
        vec4 smp = texture(iChannel0, luv);
        col = col + weight * smp.xyz;
    }
    fragColor = vec4(col, 1.0);
}
