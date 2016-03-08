#version 150 core

in vec3 in_Vertex;
in vec2 in_TexCoord0;
uniform mat4 modViewProj;
out vec2 texCoord;

void main(){
	vec3 new = in_Vertex;
	new.x /= 1000;
	new.y /= 800;
	gl_Position = modViewProj * vec4(new, 1.0);
	texCoord = in_TexCoord0;
}
