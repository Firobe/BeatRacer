#version 150 core

in vec2 texCoord;
uniform sampler2D tex0;
out vec4 out_Color;

void main(){
	out_Color = texture(tex0, texCoord);
}
