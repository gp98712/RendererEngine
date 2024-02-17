#include "Box.h"
#include "glm/glm.hpp"
#include "../../Util/stb_image.h"
#include "../MeshMaterialsLight.h"


using namespace std;

// hong lab�� �׷��Ƚ� �������� ��� ������ �������� �ۼ��Ͽ����ϴ�.
Box::Box() {
    vector<glm::vec3> positions;
    vector<glm::vec3> colors;
    vector<glm::vec3> normals;
    vector<glm::vec2> texcoords;
    // ����
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    // �Ʒ���
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    // �޸�
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
    colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    // �ո�
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    // ����
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
    colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));


    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    // ������
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    
    texcoords.push_back(glm::vec2(0.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 0.0f));
    texcoords.push_back(glm::vec2(1.0f, 1.0f));
    texcoords.push_back(glm::vec2(0.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texcoord = texcoords[i];
        vertices.push_back(v);
    }

    indices = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };
}


void Box::Draw(const char* shaderProgramName) {
    PutModelUniform(shaderProgramName);
    material.PutMaterialUniforms(shaderProgramName);
    this->mesh->Draw(shaderProgramName);
}

Box::~Box() {
    vertices.clear();
    indices.clear();
    textures.clear();
    mesh.reset();
}


