#version 400
out vec4 frag_colour;

uniform vec3 customColor;

void main() {
	frag_colour = vec4(customColor, 1.0);
}