#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuseColor1 = texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuseColor2 = texture(texture_diffuse2, TexCoords).rgb;
    vec3 diffuseColor3 = texture(texture_diffuse3, TexCoords).rgb;

    vec3 finalDiffuseColor = (diffuseColor1 + diffuseColor2 + diffuseColor3) / 3.0;
    
    vec3 specularColor1 = texture(texture_specular1, TexCoords).rgb;
    vec3 specularColor2 = texture(texture_specular2, TexCoords).rgb;

    vec3 finalSpecularColor = (specularColor1 + specularColor2) / 2.0;

    // Attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    finalDiffuseColor  *= attenuation;
    finalSpecularColor *= attenuation;

    // Diffuse Lighting
    vec3 diffuse = diff * light.diffuse * finalDiffuseColor;

    // Specular Lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * finalSpecularColor;

    vec3 ambient = light.ambient * finalDiffuseColor;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
