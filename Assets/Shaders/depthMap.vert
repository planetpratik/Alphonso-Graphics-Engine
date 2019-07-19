#version 450

layout (location = 0) in vec3 inPosition;

layout (binding = 0) uniform UBO 
{
	mat4 WorldLightViewProjection;
} ubo;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main()
{
	gl_Position =  ubo.WorldLightViewProjection * vec4(inPosition, 1.0);
}