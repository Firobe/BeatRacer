#version 150 core

in vec2 texCoord;
uniform sampler2D tex0;
uniform float state;
out vec4 out_Color;

void main(){
	vec4 color;
	if(state == 1)
		color = vec4(0.,1.,0.,1.);
	else if (state == -1)
		color = vec4(1.,0.,1.,1.);
	else color = vec4(1.,1.,1.,1.);
	out_Color = texture(tex0, texCoord) * color;
}
