// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

in vec3 normal;
in vec3 lightDir;
in vec2 texCoord;

uniform sampler2D colorMap;

out vec4 fragColor;

// Animation-wise therse is nothing special about the fragment shader, it has texture and lightning-support
void main()
{
	vec4 textureColor = texture(colorMap, texCoord);
	vec4 ambientLight = vec4(0.3, 0.3, 0.3, 1);
	float diffuseLight = clamp(dot(normalize(normal), normalize(lightDir)), 0, 1);
	fragColor = ambientLight * textureColor + diffuseLight * textureColor;
}