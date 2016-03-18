#version 150 core

in vec2 texCoord;
uniform sampler2D tex0;
uniform float calpha;
out vec4 out_Color;

void main(){
	out_Color = texture2D(tex0, texCoord).rrrr;
	if(calpha != 1)
		out_Color.a = calpha;
}
