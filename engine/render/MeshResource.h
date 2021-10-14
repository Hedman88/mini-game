#pragma once
#include "core/mathLib.h"
#include "GL/glew.h"
#include <memory>

struct Vertex
{
	Vector pos;
	Vector color;
    float uv[2];
    Vector normal;
};

struct Index
{
	GLuint v1;
	GLuint v2;
	GLuint v3;
};

class MeshResource
{
	GLuint vertexBufferHandle;
	GLuint indexBufferHandle;
	int vbsize;
	int ibsize;
public:
	void UploadToGPU(Vertex vertices[], int size, Index indices[], int size2);
	void Render();
    static std::shared_ptr<MeshResource> Cube();
    static std::shared_ptr<MeshResource> LoadObj(const char* filePath);
    static std::shared_ptr<MeshResource> PointLightCube(Vector color);
};
