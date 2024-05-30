#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor;

in vec4 iC;
in vec4 lightVector;
in vec4 normalVector;
in vec4 vVector;
in vec2 iTexCoord0;

void main(void) {

	vec4 l = normalize(lightVector);
	vec4 n = normalize(normalVector);
	vec4 v = normalize(vVector);
	vec4 r = normalize(reflect(-l, n));

	float n_dot_l = clamp(dot(n, l), 0, 1);
	float r_dot_v = pow(clamp(dot(r, v), 0, 1), 25);

	vec4 texColor = texture(textureMap0, iTexCoord0);

	pixelColor = vec4(n_dot_l * texColor.rgb, texColor.a) + vec4(r_dot_v, r_dot_v, r_dot_v, 0);
}
