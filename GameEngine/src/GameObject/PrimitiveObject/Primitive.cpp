#include "Primitive.h"
#include "../../Constants.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../Util/stb_image.h"



using namespace std;

// Assimp ����Ͽ� �� �ε��� ��쿣 ��������.
void Primitive::CalculateTangents(
) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        // Tangent ���ϴ� ����.
        // see here : https://learnopengl.com/Advanced-Lighting/Normal-Mapping
		glm::vec3 edge1 = v1.position - v0.position;
		glm::vec3 edge2 = v2.position - v0.position;

		float deltaU1 = v1.texcoord.x - v0.texcoord.x;
		float deltaV1 = v1.texcoord.y - v0.texcoord.y;
		float deltaU2 = v2.texcoord.x - v0.texcoord.x;
		float deltaV2 = v2.texcoord.y - v0.texcoord.y;

		float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

		glm::vec3 tangent;
		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

        // vertex�� �����Ǵ� ��� ������� ó���Ѵ�.
		v0.tangentModel += tangent;
		v1.tangentModel += tangent;
		v2.tangentModel += tangent;
	}

    for (size_t i = 0; i < vertices.size(); i++) {
		vertices[i].tangentModel = glm::normalize(vertices[i].tangentModel);
	}
}

Primitive::~Primitive() {
	vertices.clear();
	indices.clear();
	for (auto& texture : textures) {
		glDeleteTextures(1, &texture.id);
	}

	mesh.reset();
}

void Primitive::SetTexture(std::string path, std::string type) {
    int width, height, nrChannels;

    Texture texture;
    texture.id = 0;
    texture.type = type;
    texture.path = path;

    unsigned char* data = stbi_load(texture.path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    GLenum internalFormat;
    GLenum dataFormat;
    if (nrChannels == 1) {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }
    else if (nrChannels == 2) {
        internalFormat = GL_RG;
        dataFormat = GL_RG;
    }
    else if (nrChannels == 3) {
        internalFormat = GL_SRGB;
        dataFormat = GL_RGB;
    }
    else if (nrChannels == 4) {
        internalFormat = GL_SRGB_ALPHA;
        dataFormat = GL_RGBA;
        if (type == "albedo") {
            texture.isAlpha = true;
            isAlphaMesh = true;
        }
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(
        GL_TEXTURE_2D, // �ؽ�ó ���ε� ��� ����, ť����� GL_TEXTURE_3D�̴�.
        0, // mipmap ����
        internalFormat, // internal formap
        width,
        height,
        0, // border ũ��
        dataFormat, // �Է� ������ ����
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

void Primitive::SetupMesh() {
    CalculateTangents();

	mesh = make_shared<Mesh>(std::move(vertices), std::move(indices), std::move(textures));
	mesh->CalculateTangents();
	mesh->setupMesh();
}

void Primitive::CalculateVertexAveragePosition(glm::vec3 scale) {
	glm::vec3 sum = glm::vec3(0.0f);
    for (auto& vertex : vertices) {
		sum += vertex.position * scale;
	}
	vertexAveragePosition = sum / (float)vertices.size();
}
