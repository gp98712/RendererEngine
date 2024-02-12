#include "Box.h"
#include "glm/glm.hpp"
#include "../../Util/stb_image.h"


using namespace std;

// hong lab�� �׷��Ƚ� �������� ��� ������ �������� �ۼ��Ͽ����ϴ�.
Box::Box(float scale) {
    vector<glm::vec3> positions;
    vector<glm::vec3> colors;
    vector<glm::vec3> normals;
    vector<glm::vec2> texcoords;
    // ����
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
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
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
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

    // �ո�
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
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

    // �޸�
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
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
    positions.push_back(glm::vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f) * scale);
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
    positions.push_back(glm::vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(glm::vec3(1.0f, 1.0f, 1.0f) * scale);
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
    SetTexture();

    CalculateTangents();

    mesh = make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
    mesh->CalculateTangents();
    mesh->setupMesh();
}

void Box::Draw() {
	this->mesh->Draw();
}

void Box::SetTexture() {
    int width, height, nrChannels;

    Texture texture;
    texture.id = 0;
    texture.type = "albedo";
    texture.path = "./assets/images/wall.jpg";

    unsigned char* data = stbi_load(texture.path.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    /*
    * S: ������ (x��)
    * T: ������ (y��)
    * Ư���� �� �ִ°��� �ƴϴ�. �׳� �������� �����̶��Ѵ�.
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, // �ؽ�ó ���ε� ��� ����, ť����� GL_TEXTURE_3D�̴�.
        0, // mipmap ����
        GL_RGB, // internal formap
        width,
        height,
        0, // border ũ��
        GL_RGB, // �Է� ������ ����
        GL_UNSIGNED_BYTE, // �ȼ� ������ Ÿ��
        data // �ȼ� ������ ������.
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    Texture texture2;
    texture2.id = 0;
    texture2.type = "albedo";
    texture2.path = "./assets/images/awesomeface.png";

    glGenTextures(1, &texture2.id);
    glBindTexture(GL_TEXTURE_2D, texture2.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    unsigned char* data2 = stbi_load(texture2.path.c_str(), &width, &height, &nrChannels, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    /*
    * mipmap? : �ؽ�ó ���� ���.
    * GPU���� �ؽ�ó�� �̸� �������� ���� ���� �������� �ؽ�ó�� �����д�.
    * ex) 256* 256 -> 128 * 128 -> 64 * 64 -> 32 * 32 -> 16 * 16 -> 8 * 8 -> 4 * 4 -> 2 * 2 -> 1 * 1
    * mipmap level ���� �̿� ��Ī�ȴ�.
    * mipmap 0 -> 256 * 256
    * mipmap 1 -> 128 * 128
    *
    * ���� �Ÿ��� �� ��ü�� �������Ҷ� ���� �ػ󵵰� ���� �ؽ�ó�� �� �ʿ䰡 ������
    mipmap�� ������ �����ϸ� ����ȭ�� �ð��� ǰ���� ���� �� �ִ�.
    */
    stbi_image_free(data);
    stbi_image_free(data2);

    textures.push_back(texture);
    textures.push_back(texture2);
}

Box::~Box() {
    vertices.clear();
    indices.clear();
    textures.clear();
    mesh.reset();
}


