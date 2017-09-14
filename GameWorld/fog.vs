#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;
out vec4 Eye;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    Position = vec3(model * vec4(position, 1.0));
    TexCoords = texCoords;
    Normal = normal;  
	Eye = view * model * vec4(position, 1.0);

	gl_Position = vec4(projection * view * model * vec4(position, 1.0));
}