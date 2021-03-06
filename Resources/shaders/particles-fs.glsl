#version 330 core

in vec4 ex_color;
in vec2 texCoord;

uniform sampler2D in_texture;

out vec4 FragmentColor;

void main(void)
{
	vec4 texel = texture(in_texture, texCoord);
	if(texel.a * ex_color.a == 0.0)
		discard;

	FragmentColor = texel * ex_color;

}