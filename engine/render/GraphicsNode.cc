#include "GraphicsNode.h"
#include <cstring>

GraphicsNode::GraphicsNode(){
    this->mr = MeshResource::Cube();
    this->tr.reset(new TextureResource());
    this->sr.reset(new ShaderResource());
}

GraphicsNode::GraphicsNode(const char* objPath){
    this->mr = MeshResource::LoadObj(objPath);
    this->tr.reset(new TextureResource());
    this->sr.reset(new ShaderResource());
}

void GraphicsNode::InitNode(const char* vShaderFile, const char* pShaderFile, const char* textureFile){
    this->sr->LoadShaders(vShaderFile, pShaderFile);
    if(strcmp(textureFile, "") != 0){
        this->tr->LoadFromFile(textureFile);
    }
}

void GraphicsNode::Draw(Matrix cameraVPMatrix, Matrix modelMatrix){
    this->sr->ActivateShaders();
    this->sr->SetMatrix("matrix", (cameraVPMatrix * RotationY(0) * modelMatrix).data2);
    this->sr->SetMatrix("modelMatrix", (modelMatrix).data2);
    this->tr->BindTexture();
    this->mr->Render();
    this->tr->UnbindTexture();
}

std::shared_ptr<MeshResource> GraphicsNode::GetMR(){
    return this->mr;
}
void GraphicsNode::SetMR(std::shared_ptr<MeshResource> mr){
    this->mr = mr;
}

std::shared_ptr<TextureResource> GraphicsNode::GetTR(){
    return this->tr;
}
void GraphicsNode::SetTR(std::shared_ptr<TextureResource> tr){
    this->tr = tr;
}

std::shared_ptr<ShaderResource> GraphicsNode::GetSR(){
    return this->sr;
}
void GraphicsNode::SetSR(std::shared_ptr<ShaderResource> sr){
    this->sr = sr;
}
