#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform FragmentUniformBufferObject
{
	vec4 ambientColor;
	vec4 lightColor;
}fbo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragLightDirection;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(fragLightDirection);
	float n_dot_l = dot(lightDirection, normal);

	vec4 sampledColor = texture(texSampler, fragTexCoord);
	vec3 ambient = fbo.ambientColor.rgb * sampledColor.rgb;
	vec3 diffuse = clamp(fbo.lightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);

	outColor.rgb = ambient + diffuse;
	outColor.a = sampledColor.a;
}