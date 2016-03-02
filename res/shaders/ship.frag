#version 150 core

in vec2 texCoord;
uniform sampler2D tex0;
uniform float speed;
out vec4 out_Color;

void main(){
	out_Color = texture(tex0, texCoord) * vec4(1., 1. - speed, 1., 1.);
}
