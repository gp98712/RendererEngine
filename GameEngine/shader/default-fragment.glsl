#version 460 core
#include common.glsl


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

uniform float exposure;

in vec2 TexCoord;
in vec3 normalWorld;
in vec3 posWorld;
in vec3 tangentWorld;
in vec3 bitangentWorld;
in mat3 TBN;


in vec4 directionalLightShadowSpace;
in vec4 spotLightShadowSpace[2];

vec3 getNormal(vec3 normal,vec3 tangent,vec3 bitangent,sampler2D normalMap,vec2 TexCoords);
vec3 linearToSrgb(vec3 color);

void main() {

	vec3 toEye = normalize(cameraPos - posWorld);

	vec3 ambientColor = texture(albedo0,TexCoord).rgb;
	vec3 diffuseColor = texture(albedo0,TexCoord).rgb;
	vec3 specularColor = texture(specular0,TexCoord).rgb;

	vec3 color = vec3(0.0);

	int spotLightCount = 0;

	vec3 normal = normalWorld;

	if(use_normal0){
	    normal = getNormal(normalWorld, tangentWorld, bitangentWorld, normal0, TexCoord);
	}

	for(int i = 0; i < lightCount; i++){
	    Light light = lights[i];
		float shadow = 0.0;

		if(light.lightType == 0){

			shadow = shadowCalculation(
				directionalLightShadowSpace,
				directionalShadowDepthMap,
				normal,
				normalize(light.position - posWorld)
			);

			color += directionalLight(
				light,
				material,
				posWorld,
				normal,
				toEye,
				shadow,
				ambientColor,
				diffuseColor,
				specularColor
			);
		}
		else if(light.lightType == 2){

			shadow = shadowCalculation(
				spotLightShadowSpace[spotLightCount],
				spotShadowDepthMap[spotLightCount],
				normal,
				normalize(light.position - posWorld)
			);

			spotLightCount++;

			color += spotLight(
				light,
				material,
				posWorld,
				normal,
				toEye,
				shadow,
				ambientColor,
				diffuseColor,
				specularColor
			);
		}
	}
	
	for(int i = 0; i < pointLightCount; i++) {
		float shadow = 0.0;
		Light light = pointLights[i]; // 현재 조명

	    shadow = pointShadowCalculation(
		    posWorld,
			light,
			pointShadowDepthMap[i]
		);

		color += pointLight(
			light,
			material,
			posWorld,
			normal,
			toEye,
			shadow,
			ambientColor,
			diffuseColor,
			specularColor
		);
	}
	

	vec4 colorWithAlpha = vec4(color, texture(albedo0,TexCoord).a);

	FragColor = colorWithAlpha;

	vec3 srgbColor = linearToSrgb(FragColor.rgb);

	// TMI: 사람은 초록색에 민감하여 초록색이 높을수록 밝다고 느낀다.
	float brightness = dot(srgbColor, vec3(0.2126, 0.7152, 0.0722));

	if(brightness > 0.94){
		BloomColor = colorWithAlpha;
	}
	else{
		BloomColor = vec4(0.0,0.0,0.0,1.0);
	}
}

/*
TMI

nomalMap을 가져오는 방법엔 2가지 방법이 있다.

1. 아래 구현된것처럼 normalMap을 샘플링한 후 TBN 행렬을 사용하여 월드 좌표계로 변환하는방법.

2. 카메라, 빛의 방향을 TBN 행렬의 "역행렬"을 사용하여 월드 스페이스 -> Tangent Space로 옮겨서 계산하는 방법.

이 경우 normalMap은 Tangent Space에 있어야 함으로 TBN 행렬을 곱하지 말아야한다.

2번 방법의 경우 vertex shader에서 카메라,빛 방향을 TBN 연산을 하면 되기 때문에 효율적이지만. 직관적이지 않음.

1번 방법의 경우 fragment shader에서 TBN 행렬을 사용하기 때문에 비효율적이지만 직관적임.

나는 1번 방법을 사용하엿음.


solution: https://stackoverflow.com/questions/47620285/normal-mapping-bug
*/
vec3 getNormal(vec3 normal,vec3 tangent,vec3 bitangent,sampler2D normalMap,vec2 TexCoords){
	vec3 n = texture(normalMap, TexCoords).rgb;
	n = n * 2.0 - 1.0; // [-1 ~ 1] 정규화.
	return normalize(TBN * n);
}

vec3 linearToSrgb(vec3 color){
	vec3 mapped = vec3(1.0) - exp(-color * exposure);

	return pow(mapped, vec3(1.0/2.2));
}
