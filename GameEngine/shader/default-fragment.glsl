#version 460 core
precision highp float;
#include common.glsl
#include pbr.glsl



out vec4 FragColor;

uniform float exposure;

in vec2 TexCoord;
in vec3 normalWorld;
in vec3 posWorld;
in mat3 TBN;

in vec4 directionalLightShadowSpace;
in vec4 spotLightShadowSpace[2];

Light getNearestLight(Light nearestLight, Light light,bool isShadow,bool isInit){
    if(!isInit){
	    return light;
	}

	if(isShadow){
	    return nearestLight;
	}

	vec3 pixelToEye = normalize(cameraPos - posWorld);
	float lightDistance = length(light.position - posWorld);
	float nearestLightDistance = length(nearestLight.position - posWorld);

	return lightDistance < nearestLightDistance ? light : nearestLight;
}

void main() {

	vec3 toEye = normalize(cameraPos - posWorld);

	vec3 color = vec3(0.0);

	int spotLightCount = 0;

	vec3 normal = normalWorld;

	if(use_normal0){
	    normal = getNormal(normal0, TexCoord, TBN);
	}

	PBRValue pbrMaterial;

	vec3 pixelToEye = normalize(cameraPos - posWorld);

	vec3 ambientColor = pow(texture(albedo0,TexCoord).rgb,vec3(2.2));
	float roughness = use_roughness0 ? texture(roughness0, TexCoord).g : 0.0;
	float metallic = use_metallic0 ? texture(metallic0, TexCoord).b : 0.0;

	float ao = use_ao0 ? texture(ao0, TexCoord).r : 1.0;

	// halfWayVector ����� ����Ʈ ó������ �ٷ�.
	float ndotO = max(dot(normal, pixelToEye), 0.0);

	pbrMaterial.albedo = ambientColor;
	pbrMaterial.metallic = metallic;
	pbrMaterial.roughness = roughness;
	pbrMaterial.ao = ao;
	pbrMaterial.normalWorld = normal;
	pbrMaterial.pixelToEye = pixelToEye;
	pbrMaterial.ndotO = ndotO;
	pbrMaterial.posWorld = posWorld;
	// ndotH, ndotl, halfWay�� ����Ʈó������ �ڵ鸵.
	// ����Ʈ ó������ lightStrength�� ���������� ó�����־����.

	// GI ����� ���� ��� �������� ������ �ڵ� GI ��� �߰��� �����ؾ���.
	float countLight = 0.0;
	bool isInit = false;
	Light nearestLight;

	// �� �׸��ڰ� �¾� ������, �ƴ��� �����ؾ���.
	bool isDirectionalShadow = false;
	bool isShadow = false;

	for(int i = 0; i < lightCount; i++){
	    Light light = lights[i];
		if(light.lightType == 0){

			ShadowStruct shadow = shadowCalculation(
				directionalLightShadowSpace,
				directionalShadowDepthMap,
				normal,
				-light.direction
			);

			countLight = 1.0 - shadow.shadow;

			// GI ����� ���� ��� �������� ������ �ڵ� GI ��� �߰��� �����ؾ���.
			if(!shadow.isShadow){
				nearestLight = getNearestLight(nearestLight, light,shadow.isShadow, isInit);
				isInit = true;
			} else {
			    isDirectionalShadow = true;
			}

			color += directionalLight(
				light,
				shadow.shadow,
				pbrMaterial
			);
		}
		else if(light.lightType == 2){

			ShadowStruct shadow = shadowCalculation(
				spotLightShadowSpace[spotLightCount],
				spotShadowDepthMap[spotLightCount],
				normal,
				normalize(light.position - posWorld)
			);

			countLight = 1.0 - shadow.shadow;

			// GI ����� ���� ��� �������� ������ �ڵ� GI ��� �߰��� �����ؾ���.
			if(!shadow.isShadow){
				nearestLight = getNearestLight(nearestLight, light, shadow.isShadow, isInit);
				isInit = true;
			} else {
			  isDirectionalShadow = false;
			}

			spotLightCount++;

			color += spotLight(
				light,
				shadow.shadow,
				pbrMaterial
			);
		}
	}
	
	for(int i = 0; i < pointLightCount; i++) {
		Light light = pointLights[i]; // ���� ����

	    ShadowStruct shadow = pointShadowCalculation(
		    posWorld,
			light,
			pointShadowDepthMap[i]
		);

		// GI ����� ���� ��� �������� ������ �ڵ� GI ��� �߰��� �����ؾ���.

		if(!shadow.isShadow){
			nearestLight = getNearestLight(nearestLight, light, shadow.isShadow,isInit);
			isInit = true;
		} else {
			isDirectionalShadow = false;
		}

		
		countLight = 1.0 - shadow.shadow;

		color += pointLight(
			light,
			shadow.shadow,
			pbrMaterial
		);
	}


	vec3 ambientLight = ambientIBL(ambientColor, normal, pixelToEye, ao, metallic, roughness, countLight, pbrMaterial.ndotl);

	if(countLight == 0.0){
	    ambientLight *= vec3(0.4);
	} else if(!isDirectionalShadow) {
		float distance = length(nearestLight.position - posWorld);
		float attenuation = calcAttenuation(distance,nearestLight);
		ambientLight *= attenuation;
	}

	color += ambientLight;
	vec4 colorWithAlpha = vec4(color, texture(albedo0,TexCoord).a);

	FragColor = colorWithAlpha;
}

