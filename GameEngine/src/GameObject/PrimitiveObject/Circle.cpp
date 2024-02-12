#include "Circle.h"
#include "../../Util/stb_image.h"
#include "../../Constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Primitive.h"

// hong lab�� �׷��Ƚ� �������� ��� ������ �������� �ۼ��Ͽ����ϴ�.
Circle::Circle(float radius,
    int numSlices,
    int numStacks) {
    const float dTheta = -M_PI * 2 / float(numStacks);
    const float dPhi = -M_PI / float(numSlices);

    for (int i = 0; i <= numStacks; i++) {
        glm::vec3 stackStartPoint = glm::vec3(
            0.0f,
            -radius,
            0.0f
        );

        // Z �� �������� ȸ�� ���
        glm::mat4 zRotationMatrix = glm::rotate(
            glm::mat4(1.0f),
            dPhi * i,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        stackStartPoint = glm::vec3(
            zRotationMatrix * glm::vec4(stackStartPoint, 1.0f)
        );


        for (int j = 0; j <= numSlices; j++) {
            Vertex v;
            glm::mat4 yRotationMatrix = glm::rotate(
                glm::mat4(1.0f),
                dTheta * j,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
            v.position = glm::vec3(yRotationMatrix * glm::vec4(stackStartPoint, 1.0f));
            v.normal = glm::normalize(v.position);
            v.texcoord = glm::vec2(float(j) / numSlices, float(i) / numStacks);

            vertices.push_back(v);

        }
    }

    for (int j = 0; j < numStacks; j++) {

        const int offset = (numSlices + 1) * j;

        for (int i = 0; i < numSlices; i++) {

            indices.push_back(offset + i);
            indices.push_back(offset + i + numSlices + 1);
            indices.push_back(offset + i + 1 + numSlices + 1);

            indices.push_back(offset + i);
            indices.push_back(offset + i + 1 + numSlices + 1);
            indices.push_back(offset + i + 1);
        }
    }

    SetTexture();

    CalculateTangents();

    mesh = make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
    mesh->CalculateTangents();
    mesh->setupMesh();
}

Circle::~Circle() {
    vertices.clear();
    indices.clear();
    textures.clear();
    mesh.reset();
}

void Circle::Draw() {
    this->mesh->Draw();
}

void Circle::SetTexture() {
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