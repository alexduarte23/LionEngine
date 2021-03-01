#version 430 core

struct TE {
	mat4 a1;
	vec4 a2;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 normal[3];
layout(location = 3) in vec3 color;
layout(location = 4) in TE test;

uniform mat4 ModelMatrix;

uniform CameraMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 exColor;


void main(void) {
	vec3 t = (normal[0].xxx +normal[1].xxx+test.a1[0].xxx+test.a2.xxx);
	exColor = vec4(color+t, 1.0);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
}