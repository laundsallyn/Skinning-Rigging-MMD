R"zzz(
#version 330 core
in vec4 frag_color;
in vec4 light_direction;
in vec4 position;
out vec4 fragment_color;
void main() {
	fragment_color = frag_color;
}
)zzz"
