#shader vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in mat4 aInstanceModelMatrix; // occupies locations 2, 3, 4 and 5

out vec4 bColor;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main()
{
    bColor = aColor; // pass through color
    gl_Position = u_projection * u_view * aInstanceModelMatrix * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 bColor;

void main()
{
    FragColor = bColor;
}