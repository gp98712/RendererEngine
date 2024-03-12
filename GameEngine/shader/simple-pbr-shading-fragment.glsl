#version 460 core
#include common.glsl
// simple shading!
/*
�ּ����� �ڵ�� ���� �����ϰ� PBR ������ ���̴�.

�� ���̴� �ڵ�� 100% �Ϻ��ϰ� �������� �����Ѵ�.

directional Light�� ���ؼ��� �����Ѵ�.
*/
out vec4 FragColor;

uniform vec3 color;

in vec3 tangent;
in vec3 normal;
in vec3 biTangent;
in mat3 TBN;
in vec2 TexCoords;
in vec3 FragPos;

vec3 Fdielectric = vec3(0.04); // ��ݼ� ������ F0


float ndfGGX(float cosLh, float roughness) {
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;

    return alphaSq / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float k) {
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

float gaSchlickGGX(float cosLi, float cosLo, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

vec3 fresnelSchlick(vec3 F0,float cosTheta) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 albedo = color;
    float metallic = metallicTest;
    float roughness = roughnessTest;

    // out going light fragment -> eye
    vec3 Lo = normalize(cameraPos - FragPos);

    //vec3 N = normalize(2.0 * texture(normal0, TexCoords).rgb - 1.0);
    //N = normalize(TBN * N);

    vec3 N = normalize(normal);

    // normal dot out going light
    float cosLo = max(0.0, dot(N,Lo));

    // �ݻ籤 ����.
    vec3 Lr = reflect(-Lo, N);

    vec3 F0 = mix(Fdielectric, albedo, metallic);

    vec3 directLight = vec3(0.0);

    Light light = lights[0];
    // �𷺼ų� ����Ʈ PBR
    {
        // ����Ʈ ���� ���� ���� �������� ���� ����ϱ� ����.
        vec3 Li = -light.direction;
        vec3 Lradiance = vec3(1.0);

        // half vector
        vec3 Lh = normalize(Li + Lo);

        // n dot light ����.
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));

        vec3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
        float D = ndfGGX(cosLh, roughness);
        float G = gaSchlickGGX(cosLi, cosLo, roughness);

        vec3 kd = mix(vec3(1.0)- F0, vec3(0.0), metallic);

        vec3 diffuseBRDF = kd * albedo;

        vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

        directLight += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    }

    // IBL
    
    vec3 ambientLighting = vec3(0.0);
    
    {
        vec3 irradiance = texture(irradianceMap, N).rgb;
        vec3 F = fresnelSchlick(F0, cosLo);
        vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);

        vec3 diffuseIBL = kd * albedo * irradiance;
        
        vec3 specularIrradiance = textureLod(
        radianceMap, 
        Lr, 
        roughness * 8.0
        ).rgb;

        vec2 specularBRDF = texture(brdfTexture, vec2(cosLo, roughness)).rg;

        vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
        

        ambientLighting = diffuseIBL + specularIBL;
    }

    //ambientLighting *= 0.0;
    


    
    FragColor = vec4(directLight + ambientLighting, 1.0);
    //FragColor = vec4(directLight,1.0);
}