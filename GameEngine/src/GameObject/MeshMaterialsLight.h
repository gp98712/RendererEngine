#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Util/Shader.h"
#include "assimp/aabb.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangentModel;

	// ���� ������ ���ϱ� ����� �̻��� ������ 1.0���� �ʱ�ȭ.
	glm::vec3 color = {
		1.0f, 1.0f, 1.0f 
	};
};

/*
* �ؽ�ó�̸���.
* 
* Phong
* diffuseTexture;
* specularTexture;
* 
* PBR
* albedoTexture; <- diffuseTexture�� ��� albedo�� �̵���Ŵ.
* emissiveTexture;
* normalTexture; <- bumTexture�� ��� �̰����� �̵�
* heightTexture;
* metallicTexture;
* aoTexture;
* roughnessTexture;
* specularTexture
* 
* 
* �ؽ�ó�� �ߺ��Ǵ� ��� +1�� ���ڸ� �ٿ��� ó���Ѵ�.
* 
* example)
* albedoTexture0
* albedoTexture1 ...
* 
* ���������� PBR�� �����ϴ� ���·� ������̴�.
* ������ ������ �켱 ������ ������ �� �̰����� ����� �߰��ؾ��ϱ� ������
* ���� ���� �����Ѵ�.
* 
* �ؽ�ó�� �̸��� �и��Ѵ�.
*/

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	bool isAlpha = false;
};

extern std::vector<Texture> textures_loaded;

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	// ��� vertex���� ��� ��ġ.
	glm::vec3 position = glm::vec3(0.0f);
	bool isAlphaMesh = false;

	/*
	Mesh(std::vector<Vertex>&& vertices,
		std::vector<unsigned int>&& indices);
	*/
	Mesh(std::vector<Vertex> _vertices,
		std::vector<unsigned int> _indices,
		std::vector<Texture>_textures = {});
	~Mesh();

	void Draw(const char* shaderProgramName);
	/*
	* assimp ����ؼ� �� �ܾ�� ��쿣 ������� ����
	* Plane, Box, Sphere ���� ���� �����ϴ� vertex�� Tangents�� ����ϴ�
	* ��쿡�� ����.
	*/
	void CalculateTangents();
	void setupMesh();
	void CalculateVertexAveragePosition(glm::vec3 scale = glm::vec3(1.0f));



private:
	unsigned int VAO, VBO, EBO;
};

class Material {
public:
	glm::vec3 ambient = glm::vec3(0.1f);
	float shininess = 22.0f;
	// TMI: DirectX������ constant buffer�� ������ ������ 4byte ������ �����ϱ� ����
	// ���⿡ float dummy padding�� �־���Ѵ�.
	glm::vec3 specular = glm::vec3(1.0f);

	void PutMaterialUniforms(const char* shaderProgramName);
};
