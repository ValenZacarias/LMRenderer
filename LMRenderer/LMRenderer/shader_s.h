#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp> 

class Shader
{
public:
	unsigned int ID;

	Shader(){};
	//Constructor generates the shader on the fly
	Shader(const char* vertexPath, const char* fragmentPath)
	{

		//std::cout << "VS Path: " << vertexPath << std::endl;
		//std::cout << "FS Path: " << fragmentPath << std::endl;

		//Get the shaders source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Enable exception throw from ifstream
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//File to streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//close file handlers
			vShaderFile.close();
			fShaderFile.close();

			//strem to strings
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			//std::cout << "VS CODE: " << vShaderStream.str() << std::endl;

		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//COMPILE
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use()
	{
		glUseProgram(ID);
	}

	//SET UNIFORM VARIABLES
	void setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value)
	{
		//std::cout << "setFloat " << name << " Value: " << value << std::endl;
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setMat4(const std::string& name, glm::mat4 value)
	{
		//std::cout << "setFloat " << name << " Value: " << value << std::endl;
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setVec3(const std::string& name, float v1, float v2, float v3)
	{
		//std::cout << "setFloat " << name << " Value: " << value << std::endl;
		glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
	}

private:

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- -------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- -------------------------- -- " << std::endl;
			}
		}
	}

};

#endif