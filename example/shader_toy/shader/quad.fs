#version 330 core
out vec4 FragColor;

// uniform float iTime;
// uniform vec4 iMouse;

uniform vec2 iResolution;
// uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
// uniform float     iTimeDelta;            // render time (in seconds)
// uniform float     iFrameRate;            // shader frame rate
uniform int       iFrame;                // shader playback frame
// uniform float     iChannelTime[4];       // channel playback time (in seconds)
// uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec2      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
// uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
// uniform vec4      iDate;                 // (year, month, day, time in seconds)
// uniform float     iSampleRate;           // sound sample rate (i.e., 44100)

//create mainImage function
void mainImage( out vec4 fragColor, in vec2 fragCoord );

void main (){
    //set the color of the fragment
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    mainImage(FragColor, gl_FragCoord.xy);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    fragColor = vec4(1.0,1.0,1.0,1.0);   
}
   