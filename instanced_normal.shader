#shader vertex
#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(location = 2) in vec3 in_instanceTranslation;

out vec3 out_normal;
out vec3 out_fragPos;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(in_position + in_instanceTranslation, 1.0);

    out_normal = mat3(transpose(inverse(u_model))) * in_normal;

    out_fragPos = vec3(u_model * vec4(in_position, 1.0));
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 out_color;

in vec3 out_normal;
in vec3 out_fragPos;

uniform vec3 u_lightPos = vec3(100000.0, 100000.0, 30000.0);
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 u_viewPos;
uniform vec3 u_objectColor = vec3(0.2, 0.98, 0.78);

void main()
{
    float ambientStrength = 0.7;
    float ambientLight = ambientStrength;

    float diffusionStrength = 0.3;
    vec3 lightDir = normalize(u_lightPos - out_fragPos);
    float diffusionValue = max(dot(out_normal, lightDir), 0.0);
    float diffusedLight = diffusionStrength * diffusionValue;

    float specularStrength = 0.3;
    vec3 viewDir = normalize(u_viewPos - out_fragPos);
    vec3 reflectDir = reflect(-lightDir, out_normal);
    float specularValue = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    float specularLight = specularStrength * specularValue;

    vec3 finalColor = (ambientLight + diffusedLight + specularLight) * u_lightColor * u_objectColor;

    out_color = vec4(finalColor, 1.0);
};