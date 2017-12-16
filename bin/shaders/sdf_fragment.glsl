#version 400

in vec4 pass_color;
in vec2 pass_texcoord;

uniform sampler2D u_texture;

uniform float obj_width;
uniform float edge_width;

uniform bool enable_outline;
uniform float outline_width;
uniform float outline_edge;
uniform vec4 outline_color;
uniform vec2 outline_offset;

const float obj_edge = 0.5;
uniform bool enable_smart_edge;
uniform vec4 smart_edge_color;
uniform float smart_edge_width;
uniform float smart_edge_dist;

out vec4 out_color;

void main() {
	float distance = 1.0 - texture(u_texture, pass_texcoord).a;
	float alpha = 1.0  - smoothstep(obj_width, obj_width + edge_width, distance);
	float overall_alpha = alpha;
	vec3 overall_color = pass_color.rgb;

	// glow, shadow, outline:
	if(enable_outline) {
		float distance_border = 1.0 - texture(u_texture, pass_texcoord + outline_offset).a;
		float outline_alpha = (1.0  - smoothstep(outline_width, outline_width + outline_edge, distance_border)) * outline_color.a;

		overall_alpha = alpha + (1.0 - alpha) * outline_alpha;
		overall_color = mix(outline_color.rgb, pass_color.rgb, alpha / overall_alpha);
	}
	// smart edge:
	if(enable_smart_edge) {
		float x = distance;
		float pos = obj_edge + smart_edge_dist;
		float radius = smart_edge_width;
		float intensity = smoothstep(pos-radius, pos, x) * (1.0-smoothstep(pos, pos+radius, x));
		float smart_edge_alpha = intensity * smart_edge_color.a;
		
		if(smart_edge_alpha > 0.0) {
			overall_alpha = overall_alpha + (1.0 - overall_alpha) * smart_edge_alpha;
			overall_color = smart_edge_color.rgb;//mix(smart_edge_color.rgb, overall_color, smart_edge_alpha);
		}
		//if(smart_edge_alpha > 0.0) overall_alpha = overall_alpha + (1.0 - overall_alpha) * smart_edge_alpha;
	}

	out_color = vec4(overall_color, overall_alpha * pass_color.a);
}