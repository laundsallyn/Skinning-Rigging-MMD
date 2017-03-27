R"zzz(
#version 330 core

in vec4 light_direction;
in vec4 position;
out vec4 fragment_color;
void main() {
	fragment_color = vec4(0.8, 0.8, 0.0, 1.0);
	//fragment_color = clamp(position, 0.0, 1.0);
}
)zzz"
