#shader vertex
#version 460 core

layout(location = 0) in vec3 in_position;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 1.0);
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 finalColor = vec3(0.0, 1.0, 0.0);

    out_color = vec4(finalColor, 0.4);
};