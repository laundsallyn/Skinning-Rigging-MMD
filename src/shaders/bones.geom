R"zzz(#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position;
in vec4 vs_light_direction[];
in vec4 vs_camera_direction[];
out vec4 position;


void main() {
	int n = 0;

	position = gl_in[1].gl_Position;
	for (n = 0; n < gl_in.length(); n++) {

		gl_Position = projection * view * model * gl_in[n].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
)zzz"
