#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_color;

void main()
{
    FragColor = u_color;
}