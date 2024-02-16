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

void Box::SetupMesh() {
    CalculateTangents();

	mesh = make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
	mesh->CalculateTangents();
	mesh->setupMesh();
}

void Box::Draw(const char* shaderProgramName) {
    PutModelUniform(shaderProgramName);
    material.PutMaterialUniforms(shaderProgramName);
    this->mesh->Draw(shaderProgramName);
}

void Box::SetTexture(std::string path, std::string type) {
    int width, height, nrChannels;

    Texture texture;
    texture.id = 0;
    texture.type = type;
    texture.path = path;

    unsigned char* data = stbi_load(texture.path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    GLenum format;

    if (nrChannels == 1) {
		format = GL_RED;
	}
    else if (nrChannels == 3) {
		format = GL_RGB;
	}
    else if (nrChannels == 4) {
		format = GL_RGBA;
	}

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(
        GL_TEXTURE_2D, // �ؽ�ó ���ε� ��� ����, ť����� GL_TEXTURE_3D�̴�.
        0, // mipmap ����
        format, // internal formap
        width,
        height,
        0, // border ũ��
        format, // �Է� ������ ����
        GL_UNSIGNED_BYTE, // �ȼ� ������ Ÿ��
        data // �ȼ� ������ ������.
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    /*
    * S: ������ (x��)
    * T: ������ (y��)
    * Ư���� �� �ִ°��� �ƴϴ�. �׳� �������� �����̶��Ѵ�.
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    textures.push_back(texture);
}

Box::~Box() {
    vertices.clear();
    indices.clear();
    textures.clear();
    mesh.reset();
}


