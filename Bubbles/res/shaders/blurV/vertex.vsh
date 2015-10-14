#version 330

attribute vec3 position;
attribute vec3 rgbColor;
attribute vec2 texCoords;

varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[14];

uniform mat4 transform;
uniform vec4 overrideColor;
uniform float blurLevel;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
	
	v_texCoord = texCoords;
    v_blurTexCoords[ 0] = v_texCoord + vec2(0.0, -0.028 * blurLevel);
    v_blurTexCoords[ 1] = v_texCoord + vec2(0.0, -0.024 * blurLevel);
    v_blurTexCoords[ 2] = v_texCoord + vec2(0.0, -0.020 * blurLevel);
    v_blurTexCoords[ 3] = v_texCoord + vec2(0.0, -0.016 * blurLevel);
    v_blurTexCoords[ 4] = v_texCoord + vec2(0.0, -0.012 * blurLevel);
    v_blurTexCoords[ 5] = v_texCoord + vec2(0.0, -0.008 * blurLevel);
    v_blurTexCoords[ 6] = v_texCoord + vec2(0.0, -0.004 * blurLevel);
    v_blurTexCoords[ 7] = v_texCoord + vec2(0.0,  0.004 * blurLevel);
    v_blurTexCoords[ 8] = v_texCoord + vec2(0.0,  0.008 * blurLevel);
    v_blurTexCoords[ 9] = v_texCoord + vec2(0.0,  0.012 * blurLevel);
    v_blurTexCoords[10] = v_texCoord + vec2(0.0,  0.016 * blurLevel);
    v_blurTexCoords[11] = v_texCoord + vec2(0.0,  0.020 * blurLevel);
    v_blurTexCoords[12] = v_texCoord + vec2(0.0,  0.024 * blurLevel);
    v_blurTexCoords[13] = v_texCoord + vec2(0.0,  0.028 * blurLevel);
}