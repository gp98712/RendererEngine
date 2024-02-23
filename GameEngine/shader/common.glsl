#define MAX_LIGHTS 10

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 invTranspose;


uniform sampler2D albedo0;
uniform sampler2D specular0;


uniform samplerCube skyBox;
uniform samplerCube radianceMap;
uniform samplerCube irradianceMap;

struct Material {
    // phong shading ����.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

uniform Material material;

uniform int activeLight;

struct Light {
	vec3 direction; // Directional Light ������ �����.
    vec3 position;
    int lightType; // 0: directional, 1: point, 2: spot

    // ������
    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float cutOuter;
};

uniform Light lights[MAX_LIGHTS];

uniform vec3 cameraPos;

float calcAttenuation(float distance,Light l) {
	return 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));
}

vec3 phongShading(
    Light l,
    float lightStrength,
    vec3 toLightDirection, // ��ǥ�� <- �� �����̴�.
    vec3 normal,
    vec3 toEye,
    Material mat,
    vec3 ambientColor,
    vec3 diffuseColor,
    vec3 specularColor
    ) {
	vec3 ambient = mat.ambient * ambientColor;

    vec3 diffuse = lightStrength * mat.diffuse * diffuseColor;

    vec3 reflectDir = reflect(-toLightDirection, normal);
    float spec = pow(max(dot(toEye, reflectDir), 0.0), mat.shininess);

	vec3 specular = specularColor * mat.specular;

	return ambient + diffuse;
}

vec3 directionalLight(
    Light l, 
    Material mat,
    vec3 posWorld,
    vec3 normal,
    vec3 toEye,
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
    float intensity = clamp((theta - l.cutOuter) / epsilon, mat.ambient.r, 1.0);

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
		mat,
		ambientColor,
		diffuseColor,
		specularColor
	);
}
