#version 460 core
#include common.glsl

out vec4 FragColor;
in vec2 TexCoord;
in vec3 normalWorld;
in vec3 posWorld;
in vec3 tangentWorld;
in vec3 bitangentWorld;
in mat3 TBN;


in vec4 directionalLightShadowSpace;
in vec4 spotLightShadowSpace[2];

vec3 getNormal(vec3 normal,vec3 tangent,vec3 bitangent,sampler2D normalMap,vec2 TexCoords);

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
		Light light = pointLights[i]; // ���� ����

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
}

/*
TMI

nomalMap�� �������� ����� 2���� ����� �ִ�.

1. �Ʒ� �����Ȱ�ó�� normalMap�� ���ø��� �� TBN ����� ����Ͽ� ���� ��ǥ��� ��ȯ�ϴ¹��.

2. ī�޶�, ���� ������ TBN ����� "�����"�� ����Ͽ� ���� �����̽� -> Tangent Space�� �Űܼ� ����ϴ� ���.

�� ��� normalMap�� Tangent Space�� �־�� ������ TBN ����� ������ ���ƾ��Ѵ�.

2�� ����� ��� vertex shader���� ī�޶�,�� ������ TBN ������ �ϸ� �Ǳ� ������ ȿ����������. ���������� ����.

1�� ����� ��� fragment shader���� TBN ����� ����ϱ� ������ ��ȿ���������� ��������.

���� 1�� ����� ����Ͽ���.


solution: https://stackoverflow.com/questions/47620285/normal-mapping-bug
*/
vec3 getNormal(vec3 normal,vec3 tangent,vec3 bitangent,sampler2D normalMap,vec2 TexCoords){
	vec3 n = texture(normalMap, TexCoords).rgb;
	n = n * 2.0 - 1.0; // [-1 ~ 1] ����ȭ.
	return normalize(TBN * n);
}

