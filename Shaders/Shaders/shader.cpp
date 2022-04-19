#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::fstream vertexFile;
	std::fstream fragFile;
	std::string vertexCode;
	std::string fragCode;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragFile.open(fragmentPath);

		std::stringstream vertexStream, fragStream;
		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexCode = vertexStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR SHADER FILE NOT LOADED" << std::endl << e.what() << std::endl;
	}

	unsigned int vertex, frag;

	vertex = createShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	frag = createShader(fragCode.c_str(), GL_FRAGMENT_SHADER);

	createProgram(vertex, frag);
}

int Shader::createShader(const char* code, int type)
{
	int retVal = glCreateShader(type);
	int success;
	char infoLog[512];
	glShaderSource(retVal, 1, &code, NULL);
	glCompileShader(retVal);

	glGetShaderiv(retVal, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(retVal, 512, NULL, infoLog);
		std::cout << "Failed to compile shader:" << std::endl << infoLog << std::endl;
	}

	return retVal;
}

void Shader::createProgram(unsigned int vertex, unsigned int frag)
{
	int success;
	char infoLog[512];

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Error creating shader program:" << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}