#version 330 core

in vec4 ex_color;
in vec2 texCoord;

uniform sampler2D in_texture;
uniform sampler2D in_dissolveMap;

out vec4 FragmentColor;

void main(void)
{
	vec4 texel = texture(in_texture, texCoord);
	texel.a = clamp(texel.a * 4, 0, 1);
	//if (texel.a > 0.1) texel.a = clamp(texel.a * 2.5, 0, 1);


	vec4 color = vec4(vec3(1.0, 0.95, 0.83), texel.a);
	
	float dissolveVal = texture(in_dissolveMap, texCoord).r;
	float threshold = ex_color.w;

	if (dissolveVal > threshold)
		discard;
	else if (dissolveVal > threshold-0.1)
		color.a *= (threshold - dissolveVal) / 0.1;

	if(color.a == 0.0)
		discard;

	FragmentColor = color;
}