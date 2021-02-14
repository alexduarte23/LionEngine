#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 color;

uniform mat4 ModelMatrix;

uniform CameraMatrices {
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 exColor;


void main(void) {
	exColor = vec4(color, 1.0);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
}