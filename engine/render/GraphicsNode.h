#pragma once
#include "render/MeshResource.h"
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "core/mathLib.h"
#include <memory>

class GraphicsNode
{
    std::shared_ptr<MeshResource> mr;
    std::shared_ptr<TextureResource> tr;
    std::shared_ptr<ShaderResource> sr;

  public:
    GraphicsNode();
    GraphicsNode(const char* objPath);
    void InitNode(const char* vShaderFile, const char* pShaderFile, const char* textureFile);
    void Draw(Matrix cameraVPMatrix, Matrix modelMatrix);

    std::shared_ptr<MeshResource> GetMR();
    void SetMR(std::shared_ptr<MeshResource> mr);

    std::shared_ptr<TextureResource> GetTR();
    void SetTR(std::shared_ptr<TextureResource> tr);

    std::shared_ptr<ShaderResource> GetSR();
    void SetSR(std::shared_ptr<ShaderResource> sr);
};
