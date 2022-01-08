
#version 330 core

layout(location = 0) out vec4 color;

in vec2  v_UV;
in float v_TexSlot;
in vec4  v_Color;

uniform sampler2D u_Textures[2];

void main() {

	int index = int(v_TexSlot);
	color = texture(u_Textures[index], v_UV) * v_Color;
}