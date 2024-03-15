#version 460 core
out float FragColor; // RED만 할당되어있음.

in vec2 TexCoords;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D noiseTexture;

const int kernelSize = 64;
uniform vec3 samples[64];
uniform mat4 projection;
uniform mat4 view;

uniform float width;
uniform float height;

uniform float radius;
uniform float bias;

const vec2 noiseScale = vec2(width/4.0, height/4.0);

void main() {
    vec3 view_pos = vec3(view * vec4(texture(positionTexture,TexCoords).xyz,1.0));
    vec3 view_normal = normalize(mat3(view) * texture(normalTexture, TexCoords).xyz);

    // 이 randomVec은 현재 TangentSpace에 있는 랜덤 벡터이다.
    vec3 randomVec = normalize(texture(noiseTexture, TexCoords * noiseScale).xyz);

    /*
    아래 수학적 계산에 대한 부가 설명.

    dot(randomVec,normal) -> 투영
    normal * dot(randomVec,normal) -> 정사영

    randomVec에서 그 정사영에 대해서 빼기를 수행하면 수직인 벡터가 나온다.

    normal에 수직인 벡터이므로 tangent로 사용 가능하다.
    */
    vec3 tangent = normalize(randomVec - view_normal * dot(randomVec, view_normal));
    vec3 bitangent = cross(view_normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, view_normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; i++){
        vec3 samplePos = TBN * samples[i];
        samplePos = view_pos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5; // 텍스처 좌표계로 정규화 0~1

        vec3 sampleFragPos = vec3(view * vec4(texture(positionTexture,offset.xy).xyz,1.0));

        float sampleDepth = sampleFragPos.z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(view_pos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}
