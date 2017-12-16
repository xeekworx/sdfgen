#version 400

uniform sampler2D u_texture;
uniform bool modulate;

in vec4 pass_color;
in vec2 pass_texcoord;

out vec4 out_color;

void main() {
	if(modulate) out_color = texture(u_texture, pass_texcoord) * pass_color;
	else {
		if(textureSize(u_texture, 0).x > 0)
			out_color = texture(u_texture, pass_texcoord);
		else out_color = pass_color;
	}
}