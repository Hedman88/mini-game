#pragma once
#include "config.h"
#include "GL/glew.h"
#include <string>
#include "core/mathLib.h"

class ShaderResource
{
    GLuint program;
    const GLchar* vertexShaderCodeChar;
    const GLchar* pixelShaderCodeChar;
  public:
    int LoadShaders(const char* vShaderPath, const char* pShaderPath);
    void ActivateShaders();
    void SetMatrix(const std::string &name, float* matrix) const;
    void SetVector4(const std::string &name, Vector vec) const;
    void SetFloat(const std::string &name, float flo) const;
    void BindSampler(const std::string &name, int index) const;
};
