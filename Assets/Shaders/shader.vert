#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec3 lightDirection;
	vec3 pointLightPosition;
	float pointLightRadius;
	mat4 projectiveTextureMatrix;
	mat4 WorldLightViewProjection;
	vec3 lightPositionForShadow;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragLightDirection;
layout(location = 4) out vec3 fragWorldPosition;
layout(location = 5) out float fragPointLightAttenuation;
layout(location = 6) out vec4 fragProjectedTextureCoordinate;
layout(location = 7) out vec4 fragShadowCoordinate;
layout(location = 8) out vec3 fragLightVectorForShadow;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
	fragNormal = (ubo.model * vec4(inNormal, 0.0f)).xyz;
	fragWorldPosition = (ubo.model * vec4(inPosition, 1.0)).xyz;
	fragLightDirection = -ubo.lightDirection;

	vec3 pointLightDirection = ubo.pointLightPosition - fragWorldPosition;
	fragPointLightAttenuation = clamp(1.0f - (length(pointLightDirection) / ubo.pointLightRadius), 0.0f, 1.0f);

	fragProjectedTextureCoordinate = (vec4(inPosition, 1.0f) * ubo.projectiveTextureMatrix).xyzw;

	//fragLightVectorForShadow = normalize(ubo.lightPositionForShadow - inPosition);
	//fragShadowCoordinate =  (biasMat * ubo.WorldLightViewProjection * ubo.model)*vec4(inPosition, 1.0);
	fragLightVectorForShadow = normalize(ubo.lightPositionForShadow - fragWorldPosition);
	fragShadowCoordinate =  vec4(inPosition, 1.0) * ubo.WorldLightViewProjection;
}