#version 150


in vec2 v_texCoord;
uniform sampler2D u_diffuseTexture;
out vec4 color;

void main ()
{
	color = texture2D (u_diffuseTexture, v_texCoord);
}

