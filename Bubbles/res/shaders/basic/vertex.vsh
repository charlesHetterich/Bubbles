#version 330

attribute vec3 position;
attribute vec3 rgbColor;
attribute vec2 texCoords;

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform mat4 transform;
uniform vec4 overrideColor;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
	
	rgbColor0 = overrideColor;
	texCoords0 = texCoords;
}