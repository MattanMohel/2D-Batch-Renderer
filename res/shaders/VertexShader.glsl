
#version 330 core

layout(location = 0) in vec4  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in float a_TexSlot;
layout(location = 3) in vec2  a_UV;

out vec2  v_UV; 
out float v_TexSlot;
out vec4  v_Color;

uniform mat4 u_VP;

void main() {

// p v m pos


	gl_Position = a_Position * u_VP;
	
	v_UV       = a_UV;
	v_TexSlot  = a_TexSlot;
	v_Color    = a_Color;
}