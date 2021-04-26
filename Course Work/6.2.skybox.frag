#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform samplerCube nightbox;
uniform float blendFactor;
uniform vec3 fogColour;

const float lowerLimit = 0.0;
const float upperLimit = 20.0;

void main()
{    
    vec4 finalColour = texture(skybox,TexCoords);
    vec4 finalColour2 = texture(nightbox,TexCoords);
    vec4 mixColour = mix(finalColour,finalColour2,blendFactor);
    float factor = (TexCoords.y - lowerLimit) / (upperLimit-lowerLimit);
    factor = clamp(factor,0.0,1.0);
    FragColor = mix(vec4(fogColour,1.0),mixColour,factor);
}