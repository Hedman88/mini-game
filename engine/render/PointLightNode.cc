#include "PointLightNode.h"

PointLightNode::PointLightNode(Vector position, Vector colorRGBA, float strength){
    this->pos = position;
    this->color = colorRGBA;
    this->intensity = strength;

    this->mr = MeshResource::PointLightCube(this->color);
    this->sr.reset(new ShaderResource());
}

void PointLightNode::InitNode(const char* vShaderFile, const char* pShaderFile){
    this->sr->LoadShaders(vShaderFile, pShaderFile);
}
void PointLightNode::GiveLight(Vector cameraPos){
    this->sharedShader->ActivateShaders();
    this->sharedShader->SetVector4("lightPos", this->pos);
    this->sharedShader->SetVector4("viewPos", cameraPos);
    this->sharedShader->SetVector4("lightColor", this->color);
    this->sharedShader->SetFloat("lightIntensity", this->intensity);
}
void PointLightNode::Draw(Matrix cameraVPMatrix){
    this->sr->ActivateShaders();
    this->sr->SetMatrix("matrix", (cameraVPMatrix * PositionMat(this->pos)).data2);
    this->mr->Render();
}

std::shared_ptr<MeshResource> PointLightNode::GetMR(){
    return this->mr;
}
void PointLightNode::SetMR(std::shared_ptr<MeshResource> mr){
    this->mr = mr;
}

std::shared_ptr<ShaderResource> PointLightNode::GetSR(){
    return this->sr;
}
void PointLightNode::SetSR(std::shared_ptr<ShaderResource> sr){
    this->sr = sr;
}

std::shared_ptr<ShaderResource> PointLightNode::GetSharedShader(){
    return this->sharedShader;
}
void PointLightNode::SetSharedShader(std::shared_ptr<ShaderResource> sharedShader){
    this->sharedShader = sharedShader;
}
