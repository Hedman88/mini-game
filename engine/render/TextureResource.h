#pragma once
#include "render/stb_image.h"
#include "GL/glew.h"

class TextureResource
{
    GLuint texture;
public:
    ~TextureResource();
    void BindTexture();
    void UnbindTexture();
    void LoadFromFile(const char* filename);
};
