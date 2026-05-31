uniform sampler2D texture;
uniform float time;
uniform float fear;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    // Distorted security camera lens fisheye
    vec2 distUV = uv - 0.5;
    float r2 = distUV.x * distUV.x + distUV.y * distUV.y;
    distUV *= 1.0 + 0.08 * r2;
    vec2 targetUV = distUV + 0.5;
    
    vec4 color;
    if (targetUV.x < 0.0 || targetUV.x > 1.0 || targetUV.y < 0.0 || targetUV.y > 1.0) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        color = texture2D(texture, targetUV);
    }

    // Animated fog overlay replacing particles
    float fogNoise = sin(targetUV.x * 6.0 + time * 0.5) * cos(targetUV.y * 4.0 + time * 0.3) + 
                     sin(targetUV.x * 3.0 - time * 0.2) * cos(targetUV.y * 7.0 + time * 0.4);
    fogNoise = clamp((fogNoise + 2.0) * 0.25, 0.0, 1.0);
    vec3 fogColor = vec3(0.05, 0.1, 0.15); // Cold blue fog
    float fogAlpha = (0.2 + 0.15 * fear) * fogNoise;
    
    color.rgb = mix(color.rgb, fogColor, fogAlpha);

    // Vignette (tightens with fear)
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(targetUV, center);
    float vignette = smoothstep(0.92 - 0.28 * fear, 0.35 - 0.20 * fear, dist);

    // CRT scanlines
    float scanline = 1.0 - 0.10 * abs(sin(uv.y * 1080.0 + time * 5.0));

    // Film grain (more at high fear)
    float noise = (fract(sin(dot(targetUV + time, vec2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.18 * fear;

    // Chromatic aberration at high fear
    float aberration = 0.0;
    if (fear > 0.5) {
        aberration = (fear - 0.5) * 0.004;
    }
    if (aberration > 0.0) {
        float rOff = texture2D(texture, targetUV + vec2(aberration, 0.0)).r;
        float bOff = texture2D(texture, targetUV - vec2(aberration, 0.0)).b;
        color.r = mix(color.r, rOff, 0.5);
        color.b = mix(color.b, bOff, 0.5);
    }

    color.rgb *= vignette * scanline;
    color.rgb += vec3(noise);

    // Blood red desaturation at high fear
    if (fear > 0.4) {
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        vec3 bloodRed = vec3(gray * 1.4, gray * 0.35, gray * 0.35);
        color.rgb = mix(color.rgb, bloodRed, (fear - 0.4) * 1.6);
    }

    // Subtle pulse breathing at medium fear
    if (fear > 0.2) {
        float pulse = 1.0 - 0.04 * (fear - 0.2) * sin(time * 2.5);
        color.rgb *= pulse;
    }

    gl_FragColor = color * gl_Color;
}