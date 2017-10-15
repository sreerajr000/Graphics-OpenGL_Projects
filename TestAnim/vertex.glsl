// Written by Karl Zylinski
// Portfolio and website: http://zylinski.se
// Use, modify and copy in any way you want

#version 330

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec4 vInfluences;
in vec4	vWeights;
in float vNumWeights;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;
uniform mat4 bones[250];	// All bones come in a huge list!

out vec3 normal;
out vec3 lightDir;
out vec2 texCoord;

void main()
{
	texCoord = vTexCoord;
	normal = vNormal;

	// Used to mesure how much weight we've added together, should end up at 1.0
	float totalWeight = 0;

	// This is the position and normal which will be calculated using the influencing bones
	vec4 position = vec4(0,0,0,1);
	vec3 normal = vec3(0,0,0);

	// vNumWeights will be a number between 1 and 3
	for(int i = 0; i < int(vNumWeights); i++)
	{
		// A weight is a number beteen 0 and 1, the number fetched from the influence list is the index in the bones array
		position += vWeights[i] * vec4(bones[int(vInfluences[i])] * vPosition);
		normal += vWeights[i] * vec3(bones[int(vInfluences[i])] * vec4(vNormal,1));

		totalWeight += vWeights[i];
	}
	
	// totalWeight _should_ be 1, we adjust the position just in case it isn't
	position /= totalWeight;
	position.w = 1; // w will problaby have a weird value after all those additions
	normalize(normal);

	lightDir = normalize(lightPosition - position.xyz);
	gl_Position = mvpMatrix * position;
}