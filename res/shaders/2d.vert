#version 150 core
#
in vec2 in_Vertex;
in vec2 in_TexCoord0;
uniform mat4 modViewProj;
uniform vec2 screen;
out vec2 texCoord;

void main(){
	vec2 new = vec2(in_Vertex.x / screen.x, in_Vertex.y / screen.y);
	gl_Position = modViewProj * vec4(new, 0., 1.);
	texCoord = in_TexCoord0;
}
