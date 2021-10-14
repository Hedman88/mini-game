#pragma once
#include "render/MeshResource.h"
#include "render/ShaderResource.h"
#include "core/mathLib.h"
#include <memory>

class PointLightNode
{
    Vector pos;
    Vector color;
    float intensity;

    std::shared_ptr<MeshResource> mr;
    std::shared_ptr<ShaderResource> sr;
    std::shared_ptr<ShaderResource> sharedShader;
  public:
    PointLightNode(Vector position, Vector colorRGBA, float strength);
    void InitNode(const char* vShaderFile, const char* pShaderFile);
    void GiveLight(Vector cameraPos);
    void Draw(Matrix cameraVPMatrix);

    std::shared_ptr<MeshResource> GetMR();
    void SetMR(std::shared_ptr<MeshResource> mr);

    std::shared_ptr<ShaderResource> GetSR();
    void SetSR(std::shared_ptr<ShaderResource> sr);

    std::shared_ptr<ShaderResource> GetSharedShader();
    void SetSharedShader(std::shared_ptr<ShaderResource> sharedShader);
};
