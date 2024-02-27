#define MAX_LIGHTS 5

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 invTranspose;


uniform sampler2D albedo0;
uniform sampler2D specular0;


uniform samplerCube skyBox;
uniform samplerCube radianceMap;
uniform samplerCube irradianceMap;

struct NormalShadowMap {
	sampler2D depthMap;
    bool use;
    mat4 lightSpaceMatrix;
};

struct PointShadowMap {
	samplerCube depthMap;
    bool use;
	mat4 lightSpaceMatrix;
};

uniform NormalShadowMap directionalShadowMap;
uniform NormalShadowMap spotShadowMap[2];
uniform PointShadowMap pointShadowMap[2];

struct Material {
    // phong shading ����.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;

    vec3 fresnelIRO;
};

uniform Material material;

uniform int lightCount;

struct Light {
	vec3 direction; // Directional Light ������ �����.
    vec3 position;
    int lightType; // 0: directional, 1: point, 2: spot

    vec3 strength; // ���� ����

    // ������
    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float cutOuter;
};

uniform Light lights[MAX_LIGHTS];

uniform vec3 cameraPos;

vec3 schlickFresnel(vec3 iro, vec3 normal, vec3 toEye); // ���漱��

float calcAttenuation(float distance,Light l) {
	return 1.0 / distance;
}

vec3 ambientIBL(Material material,vec3 normal, vec3 toEye) {
    vec4 diffuse = texture(radianceMap, normal);
    vec4 specular = texture(irradianceMap,reflect(-toEye,normal));

    specular *= pow(
        (specular.x + specular.y + specular.z) / 3.0,
        material.shininess
    );

    return (specular + diffuse).rgb;
}

vec3 phongShading(
    Light l,
    float lightStrength,
    vec3 toLightDirection, // ��ǥ�� <- �� �����̴�.
    vec3 normal,
    vec3 toEye,
    float shadow,
    Material mat,
    vec3 ambientColor,
    vec3 diffuseColor,
    vec3 specularColor
    ) {
    vec3 halfWayDir = normalize(toLightDirection + toEye);

	vec3 ambient = ambientIBL(mat,normal,toEye) * mat.ambient * ambientColor;

    vec3 diffuse = lightStrength * mat.diffuse * diffuseColor * l.strength;

    float spec = pow(max(dot(normal, halfWayDir), 0.0), mat.shininess);

	vec3 specular = specularColor * mat.specular * l.strength * spec;

    vec3 f = schlickFresnel(mat.fresnelIRO,normal,toEye);

    specular *= f;

	return ambient + (diffuse + specular) * (1.0 - shadow);
}

vec3 directionalLight(
    Light l, 
    Material mat,
    vec3 posWorld,
    vec3 normal,
    vec3 toEye,
    float shadow,
    vec3 ambientColor,
    vec3 diffuseColor,
    vec3 specularColor) {

    vec3 lightVec = normalize(-l.direction);

    float lightStrength = max(dot(normal, lightVec), 0.0);

    return phongShading(
        l,
        lightStrength,
        lightVec,normal,
        toEye,
        shadow,
        mat,
        ambientColor,
        diffuseColor,
        specularColor
    );
}

vec3 pointLight(
    Light l, 
    Material mat,
    vec3 posWorld,
    vec3 normal,
    vec3 toEye,
    float shadow,
    vec3 ambientColor,
    vec3 diffuseColor,
    vec3 specularColor
) {
	vec3 toLight = normalize(l.position - posWorld);
    float lightStrength = max(dot(normal, toLight), 0.0);
    float distance = length(l.position - posWorld);
    float attenuation = calcAttenuation(distance,l);

    ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return phongShading(
        l,
        lightStrength,
        toLight,
        normal,
        toEye,
        shadow,
        mat,
        ambientColor,
        diffuseColor,
        specularColor
    );
}

vec3 spotLight(    
    Light l, 
    Material mat,
    vec3 posWorld,
    vec3 normal,
    vec3 toEye,
    float shadow,
    vec3 ambientColor,
    vec3 diffuseColor,
    vec3 specularColor
    ) {
    vec3 lightVec = normalize(l.position - posWorld);

    float theta = dot(lightVec, normalize(-l.direction));

    // ��� ������ Intensity ���� ����
    // Phi = ����Ʈ����Ʈ�� ����� �޴� ���� (cutOff)
    // Theta = LightDirection�� LightVec�� ���� ���� ���� ���� �ȼ��� ����Ʈ����Ʈ ������ �����̴�.
    // gamma = �ܺ� ����, �Ѿ�� �ܺ� ���� ���� ���� (cutOuter)
    // epsilon = �ܺ� ������ ���� ���� ������ ���� cutOff - cutOuter "�������̴�."
    // Intensity = (Theta - cutOuter) / (cutOff - cutOuter)
    // Theta�� Phi�� ���ٸ� (����Ʈ����Ʈ�� ����) Intensity = 1
    // Theta�� �������� ũ�ٸ� ������ �Ǿ� clamp�� ���� Intensity = 0;
    // Theta�� Phi���� Phi���� �۴ٸ� Intensity�� ��� 1���� ū ���� �Ǿ� clamp�� ���� 1�̵ȴ�.
    // Theta�� Phi���� ũ�� Theta���� ������ ��μ� 0~1 ������ ���� ���´�.

    float epsilon = l.cutOff - l.cutOuter;
    // as-is intensity �ִ밪 1
    // to-be intensity �ִ밪 = 5
    // �ִ밪�� 1�̸� ����Ʈ����Ʈ�� �ʹ� ���ϰ� ���� �Ѱ踦 �÷��־���.
    float intensity = clamp((theta - l.cutOuter) / epsilon, mat.ambient.r, 5.0);

    float lightStrength = max(dot(normal, lightVec), 0.0);
    float distance = length(l.position - posWorld);
    float attenuation = calcAttenuation(distance,l);

    ambientColor *= attenuation * intensity;
    diffuseColor *= attenuation * intensity;
    specularColor *= attenuation * intensity;

    return phongShading(
        l,
		lightStrength,
		lightVec,
        normal,
		toEye,
        shadow,
		mat,
		ambientColor,
		diffuseColor,
		specularColor
	);
}

// RGB�� HSV�� ��ȯ�ϴ� �Լ�
vec3 rgb2hsv(vec3 c) {
    vec4 K = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// HSV�� RGB�� ��ȯ�ϴ� �Լ�
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float shadowCalculation(vec4 fragPosLightSpace, NormalShadowMap shadowMap,vec3 normal, vec3 lightDir) {
    float bias = 0.001;

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // z���� ���� ���� ���� �����´�. ť��� �ٴ��� ��¦ �پ������� �׸��ڰ� ������ �ʴ� �̽��� �ִ�.
    projCoords.z = projCoords.z + 0.01;

    float closestDepth = texture(shadowMap.depthMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMap.depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap.depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

    return shadow / 9.0;
}

/*
https://psgraphics.blogspot.com/2020/03/fresnel-equations-schlick-approximation.html

vec3 R0 = (guess by looking)
vec3 R = R0 + (1-R0)*pow(1-cosine,5)
*/
vec3 schlickFresnel(vec3 iro, vec3 normal, vec3 toEye) {
    float lookAngle = dot(normal,toEye);
    // 90���� ������ ������ ����, 0���� ������ �ݻ���� ��ȯ.
    lookAngle = 1.0 - clamp(lookAngle,0.0,1.0);

    return iro + (1.0f - iro) * pow(lookAngle,5.0);
}
