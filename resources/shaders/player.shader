#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
    gl_Position = u_MVP * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

void main() {
    color = vec4(0.5f, 0.5f, 1.0f, 1.0f);
}
