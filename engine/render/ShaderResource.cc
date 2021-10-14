#include "ShaderResource.h"
#include <fstream>
#include <sstream>
#include <iostream>

int ShaderResource::LoadShaders(const char* vShaderPath, const char* pShaderPath){
    std::string vShaderCode;
    std::string pShaderCode;
    std::ifstream vShaderFile;
    std::ifstream pShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    pShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        vShaderFile.open(vShaderPath);
        pShaderFile.open(pShaderPath);
        std::stringstream vShaderStream, pShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        pShaderStream << pShaderFile.rdbuf();
        vShaderFile.close();
        pShaderFile.close();
        vShaderCode = vShaderStream.str();
        pShaderCode = pShaderStream.str();
    }catch(std::ifstream::failure e){
        std::cout << "ERROR: failed to read shader file, check ShaderResource.cc" << std::endl;
    }

    this->vertexShaderCodeChar = vShaderCode.c_str();
    this->pixelShaderCodeChar = pShaderCode.c_str();

    GLuint vertexShader, pixelShader;
    GLint success;
    GLchar infoLog[512];

    // Setup vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &this->vertexShaderCodeChar, NULL);
    glCompileShader(vertexShader);

    // Getting error log
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: vertex shader failed to compile\n" << infoLog << std::endl;
    }

    // Setup pixel shader
    pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(pixelShader, 1, &this->pixelShaderCodeChar, NULL);
    glCompileShader(pixelShader);

    // Getting error log
    glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(pixelShader, 512, NULL, infoLog);
        std::cout << "ERROR: pixel shader failed to compile\n" << infoLog << std::endl;
    }

    // Creating program object and linking shaders
    this->program = glCreateProgram();
    glAttachShader(this->program, vertexShader);
    glAttachShader(this->program, pixelShader);
    glLinkProgram(this->program);

    // Getting error log
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
    }

    // Shaders already linked, clearing memory
    glDeleteShader(vertexShader);
    glDeleteShader(pixelShader);
    return 0;
}

void ShaderResource::ActivateShaders(){
    glUseProgram(this->program);
}

void ShaderResource::SetMatrix(const std::string &name, float* matrix) const{
	glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, true, matrix);
}

void ShaderResource::SetVector4(const std::string &name, Vector vec) const{
    glUniform4fv(glGetUniformLocation(this->program, name.c_str()), 1, vec.arr);
}

void ShaderResource::BindSampler(const std::string &name, int index) const{
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), index);
}

void ShaderResource::SetFloat(const std::string &name, float flo) const{
    glUniform1f(glGetUniformLocation(this->program, name.c_str()), flo);
}
