#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform ProxyModelUniformBufferObject {
    mat4 mvp;
} pmubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = pmubo.mvp * vec4(inPosition, 1.0);
    fragColor = vec3(0.5,0.0,0.0);
}