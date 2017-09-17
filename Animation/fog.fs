#version 430

out vec4 color;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;
in vec4 Eye;

const float fogDensity = 0.1f;
const vec4 fogColor(0.5, 0.5, 0.5, 1.0);

uniform sampler2D tex;


float getFogFactor(float fogCoord)
{
	float result = 0.0;

	result = exp(-fogDensity*fogCoord);
	result = 1.0f - clamp(result, 0.0f, 1.0f);

	return result;
}


void main()
{
	vec4 col += texture(tex, TexCoords);

	float fogCoord = abs(Eye.z / Eye.w);
	color = mix(col, fogColor, getFogFactor(fogCoord));	
}