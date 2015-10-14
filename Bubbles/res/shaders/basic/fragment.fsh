#version 330

varying vec4 rgbColor0;
varying vec2 texCoords0;

uniform sampler2D diffuse;

void main()
{	
	vec4 endResult =  texture(diffuse, vec2(texCoords0.x, texCoords0.y)) * vec4(rgbColor0);
	
	if (endResult.a <= 0)
		discard;
	
	gl_FragColor = endResult;
}