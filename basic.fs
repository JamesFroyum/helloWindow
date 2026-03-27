#version 330 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;

void main() { //here we have a giant mess that combines two textures, then mulitplies them by the color
	FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), mixValue) * ourColor;
}