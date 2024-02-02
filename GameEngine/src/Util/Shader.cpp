#include "Shader.h"

Shader* Shader::instance = nullptr;

std::string Shader::readShaderSource(const char* filePath)
{
	std::ifstream shaderFile;
    /*
    * failbit <- I/O �۾��� ���� ���� �߻�
    * badbit <- �б� ���� ������ ���� ����
    */
	shaderFile.exceptions(
		std::ifstream::failbit | std::ifstream::badbit
	);
    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        // ���� ������ ��Ʈ���� �о�ɴϴ�.
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        // ��Ʈ���� std::string���� ��ȯ�մϴ�.
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        return "";
    }
}

GLuint Shader::compileShader(const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // ������ ���� Ȯ��
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint Shader::getVertexShader(const char* filePath) {
    auto vertexShader = shaderMap.find(filePath);

    if (vertexShader == shaderMap.end()) {
        std::string vertexShaderSource = readShaderSource(filePath);

        GLuint vertexShaderID = compileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
        shaderMap[filePath] = vertexShaderID;
        return vertexShaderID;
    }

    return vertexShader->second;
}

GLuint Shader::getFragmentShader(const char* filePath) {
    auto fragmentShader = shaderMap.find(filePath);

	if (fragmentShader == shaderMap.end()) {
		std::string fragmentShaderSource = readShaderSource(filePath);
		GLuint fragmentShaderID = compileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
		shaderMap[filePath] = fragmentShaderID;
		return fragmentShaderID;
	}

	return fragmentShader->second;
}

unsigned int Shader::loadShaderProgram(
    const char* vertexShaderPath,
    const char* fragmentShaderPath,
    const char* shaderProgramName
) {
    auto shaderProgramId = shaderProgramMap.find(shaderProgramName);
    auto vertexShaderId = getVertexShader(vertexShaderPath);
    auto fragmentShaderId = getFragmentShader(fragmentShaderPath);

    if(shaderProgramId == shaderProgramMap.end()) {
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShaderId);
		glAttachShader(shaderProgram, fragmentShaderId);
		glLinkProgram(shaderProgram);

		// ��ũ ���� Ȯ��
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		shaderProgramMap[shaderProgramName] = shaderProgram;

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        shaderMap.erase(vertexShaderPath);
        shaderMap.erase(fragmentShaderPath);

		return shaderProgram;
	}

    return shaderProgramId->second;
}

unsigned int Shader::getShaderProgram(const char* shaderProgramName) {
	auto shaderProgramId = shaderProgramMap.find(shaderProgramName);

	if (shaderProgramId == shaderProgramMap.end()) {
		std::cerr << "ERROR::SHADER::PROGRAM::NOT_FOUND" << std::endl;
		return 0;
	}

	return shaderProgramId->second;
}
