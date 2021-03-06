#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform FragmentUniformBufferObject
{
	vec4 ambientColor;
	vec4 lightColor;
	vec4 pointLightColor;
	vec3 pointLightPosition;
	vec3 cameraPosition;
	vec4 specularColor;
	float specularPower;
}fbo;

layout(binding = 3) uniform sampler2D projectedTexSampler;
layout(binding = 4) uniform sampler2D ShadowMapSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragLightDirection;
layout(location = 4) in vec3 fragWorldPosition;
layout(location = 5) in float fragPointLightAttenuation;
layout(location = 6) in vec4 fragProjectedTextureCoordinate;
layout(location = 7) in vec4 fragShadowCoordinate;
layout(location = 8) in vec3 fragLightVectorForShadow;


layout(location = 0) out vec4 outColor;

float textureProj(vec4 shadowCoord, vec2 off)
{
	float shadow = 1.0;
	if ( shadowCoord.z > -1.0 && shadowCoord.z < 1.0 ) 
	{
		float dist = texture( ShadowMapSampler, shadowCoord.st + off ).r;
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) 
		{
			//shadow = 0.1;
		}
	}
	return shadow;
}

void main() 
{
	//vec4 whiteColor = vec4(0,0,1,1);
	//vec4 blackColor = vec4(0,1,0,1);
	vec4 whiteColor = vec4(1,1,1,1);
	vec4 blackColor = vec4(0.71,0.71,0.71,0.5);
	vec3 pointLightDirection = normalize(fbo.pointLightPosition - fragWorldPosition);
	vec3 viewDirection = normalize(fbo.cameraPosition - fragWorldPosition);

	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(fragLightDirection);

	float n_dot_l = dot(lightDirection, normal);
	float n_dot_l_pointLight = dot(pointLightDirection, normal);
	float n_dot_l_lightForShadow = dot(fragLightVectorForShadow, normal);

	vec3 halfVector = normalize(pointLightDirection + viewDirection);
	float n_dot_h_pointLight = dot(normal, halfVector);

	vec4 sampledColor = texture(texSampler, fragTexCoord);
	vec3 ambient = fbo.ambientColor.rgb * sampledColor.rgb;
	vec3 diffuse = clamp(fbo.lightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);

	vec3 diffusePointLight = clamp(fbo.pointLightColor.rgb * n_dot_l_pointLight * sampledColor.rgb, 0.0f, 1.0f) * fragPointLightAttenuation;
	vec3 specular = fbo.specularColor.rgb * min(pow(clamp(n_dot_h_pointLight, 0.0f, 1.0f), fbo.specularPower), sampledColor.w) * fragPointLightAttenuation;

	vec3 diffuseLightForShadow = clamp(fbo.lightColor.rgb * n_dot_l_lightForShadow * sampledColor.rgb, 0.0f, 1.0f)*fragPointLightAttenuation;

	outColor.rgb = ambient + diffuse + diffusePointLight + diffuseLightForShadow + specular;
	outColor.a = sampledColor.a;

	if(fragProjectedTextureCoordinate.w <= 0.0f)
	{
		vec2 projectedTextureCoordinate = fragProjectedTextureCoordinate.xy / fragProjectedTextureCoordinate.w;
		vec3 sampledProjectedTexColor = texture(projectedTexSampler, projectedTextureCoordinate.xy).rgb;
		outColor.rgb *= sampledProjectedTexColor;
	}

	//float shadow = textureProj(fragShadowCoordinate / fragShadowCoordinate.w, vec2(0.0));
	//outColor.rgb *= shadow;

	if(fragShadowCoordinate.w <= 0.0f)
	{
		vec3 shadowCoordinate = fragShadowCoordinate.xyz / fragShadowCoordinate.w;
		float pixelDepth = shadowCoordinate.z;
		float sampledDepth = texture(ShadowMapSampler, shadowCoordinate.xy).x + 0.05;
		vec3 shadow = (pixelDepth > sampledDepth ? blackColor.rgb : whiteColor.rgb);
		outColor.rgb *= shadow;
	}
	
}