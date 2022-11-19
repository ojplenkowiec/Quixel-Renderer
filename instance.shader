#shader vertex
#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(location = 2) in vec3 in_instanceTranslation;
layout(location = 3) in vec3 in_instanceColor;

out vec3 out_normal;
out vec3 out_fragPos;
out vec3 out_instanceColor;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_position + in_instanceTranslation, 1.0);

    out_normal = mat3(transpose(inverse(u_model))) * in_normal;

    out_fragPos = vec3(u_model * vec4(in_position, 1.0));
    out_instanceColor = in_instanceColor;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 out_normal;
in vec3 out_fragPos;
in vec3 out_instanceColor;

uniform vec3 u_lightPos = vec3(0.0, 700.0, 0.0);
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 u_viewPos;
uniform vec3 u_objectColor = vec3(1.0, 1.0, 1.0);

void main()
{
    float ambientStrength = 1.15;
    float ambientLight = ambientStrength;

    float diffusionStrength = 0.6;
    vec3 lightDir = normalize(u_lightPos - out_fragPos);
    float diffusionValue = max(dot(out_normal, lightDir), 0.0);
    float diffusedLight = diffusionStrength * diffusionValue;

    float specularStrength = 0.9;
    vec3 viewDir = normalize(u_viewPos - out_fragPos);
    vec3 reflectDir = reflect(-lightDir, out_normal);
    float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    float specularLight = specularStrength * specularValue;

    vec3 finalColor = (ambientLight + diffusedLight + specularLight) * u_lightColor * out_instanceColor;

    out_color = vec4(finalColor, 1.0);
};