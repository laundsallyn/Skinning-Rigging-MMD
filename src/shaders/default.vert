R"zzz(
#version 330 core
uniform vec4 light_position;
uniform vec3 camera_position;
in vec4 vertex_position;
in vec4 normal;
in vec2 uv;
layout (location = 1) in vec4 color;
out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;
out vec4 vs_color;
void main() {
	gl_Position = vertex_position;
	vs_light_direction = light_position - gl_Position;
	vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	vs_normal = normal;
	vs_uv = uv;
	vs_color = color;
}
)zzz"
