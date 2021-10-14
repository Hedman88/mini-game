#include "config.h"
#include "MeshResource.h"
#include <vector>
#include <cstring>
#include <map>

void MeshResource::UploadToGPU(Vertex vertices[], int size, Index indices[], int size2) {
	glGenBuffers(1, &this->vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->vbsize = size;

	glGenBuffers(1, &this->indexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * size2, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	this->ibsize = size2 * 3;
}

void MeshResource::Render() {

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*8));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*10));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferHandle);

	glDrawElements(GL_TRIANGLES, ibsize, GL_UNSIGNED_INT, NULL);
}

std::shared_ptr<MeshResource> MeshResource::Cube() {

    //MeshResource cube = MeshResource();
    std::shared_ptr<MeshResource> cube(new MeshResource());
	Vertex vertices[24] = {	
		// Front
		Vertex{Vector(-0.5, -0.5, 0.5), Vector(1,0,0,1), {0.5, 0.75}},
		Vertex{Vector(-0.5, 0.5, 0.5), Vector(1,0,0,1), {0.75, 0.75}},
		Vertex{Vector(0.5, -0.5, 0.5), Vector(1,0,0,1), {0.5, 1.0}},
		Vertex{Vector(0.5, 0.5, 0.5), Vector(1,0,0,1), {0.75, 1.0}},

		// Left
		Vertex{Vector(-0.5, -0.5, 0.5), Vector(0,1,0,1), {0.25, 0.5}},
		Vertex{Vector(-0.5, 0.5, 0.5), Vector(0,1,0,1), {0.5, 0.5}},
		Vertex{Vector(-0.5, -0.5, -0.5), Vector(0,1,0,1), {0.25, 0.75}},
		Vertex{Vector(-0.5, 0.5, -0.5), Vector(0,1,0,1), {0.5, 0.75}},

		// Right
		Vertex{Vector(0.5, -0.5, -0.5), Vector(0,0,1,1), {0.75, 0.5}},
		Vertex{Vector(0.5, 0.5, -0.5), Vector(0,0,1,1), {1.0, 0.5}},
		Vertex{Vector(0.5, -0.5, 0.5), Vector(0,0,1,1), {0.75, 0.75}},
		Vertex{Vector(0.5, 0.5, 0.5), Vector(0,0,1,1), {1.0, 0.75}},

		// Over
		Vertex{Vector(-0.5, 0.5, -0.5), Vector(1,1,1,1), {0.5, 0.5}},
		Vertex{Vector(-0.5, 0.5, 0.5), Vector(1,1,1,1), {0.75, 0.5}},
		Vertex{Vector(0.5, 0.5, -0.5), Vector(1,1,1,1), {0.5, 0.75}},
		Vertex{Vector(0.5, 0.5, 0.5), Vector(1,1,1,1), {0.75, 0.75}},

		// Under
		Vertex{Vector(-0.5, -0.5, 0.5), Vector(0,0,0,1), {0.0, 0.5}},
		Vertex{Vector(-0.5, -0.5, -0.5), Vector(0,0,0,1), {0.25, 0.5}},
		Vertex{Vector(0.5, -0.5, 0.5), Vector(0,0,0,1), {0.0, 0.75}},
		Vertex{Vector(0.5, -0.5, -0.5), Vector(0,0,0,1), {0.25, 0.75}},

		// Back
		Vertex{Vector(-0.5, -0.5, -0.5), Vector(1,0,1,1), {0.5, 0.25}},
		Vertex{Vector(-0.5, 0.5, -0.5), Vector(1,0,1,1), {0.75, 0.25}},
		Vertex{Vector(0.5, -0.5, -0.5), Vector(1,0,1,1), {0.5, 0.5}},
		Vertex{Vector(0.5, 0.5, -0.5), Vector(1,0,1,1), {0.75, 0.5}}
		};

	Index indices[12] = {
		Index{0,1,3},
		Index{0,2,3},

		Index{4,5,7},
		Index{4,6,7},
		
		Index{8,9,11},
		Index{8,10,11},
		
		Index{12,13,15},
		Index{12,14,15},
		
		Index{16,17,19},
		Index{16,18,19},
		
		Index{20,21,23},
		Index{20,22,23}
	};
	cube->UploadToGPU(vertices, 24, indices, 12);
    return cube;
}

std::shared_ptr<MeshResource> MeshResource::LoadObj(const char* filePath){
    std::shared_ptr<MeshResource> objMesh(new MeshResource());
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    std::vector<Vector> temp_verts;
    std::vector<Vector> temp_uvs;
    std::vector<Vector> temp_normals;

    int vertexAmount = 0;

    // Open the file
    FILE* file = fopen(filePath, "r");
    if(file == NULL){
        std::cout << "ERROR: opening file, check MeshResource.cc" << std::endl;
        return nullptr;
    }
    // Read the file
    while(true){
        char firstWord[128];
        // Reading the first word of the line
        int res = fscanf(file, "%s", firstWord);
        if(res == EOF) break;

        if(strcmp(firstWord, "v") == 0){
            Vector vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_verts.push_back(vertex);
        }else if(strcmp(firstWord, "vt") == 0){
            Vector uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }else if(strcmp(firstWord, "vn") == 0){
            Vector normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normal.w = 0;
            temp_normals.push_back(normal);
        }else if(strcmp(firstWord, "f") == 0){
            GLuint vertexIndex[4], uvIndex[4], normalIndex[4];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2],
                                 &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            if(matches == 9 || matches == 12){
                vertexAmount += 3;

                Vertex vert;
                vert.pos = temp_verts[vertexIndex[0] - 1];
                vert.pos.w = 1;
                vert.uv[0] = temp_uvs[uvIndex[0] - 1].x;
                vert.uv[1] = temp_uvs[uvIndex[0] - 1].y;
                vert.normal = temp_normals[normalIndex[0] - 1];
                vertices.push_back(vert);

                Vertex vert2;
                vert2.pos = temp_verts[vertexIndex[1] - 1];
                vert2.pos.w = 1;
                vert2.uv[0] = temp_uvs[uvIndex[1] - 1].x;
                vert2.uv[1] = temp_uvs[uvIndex[1] - 1].y;
                vert2.normal = temp_normals[normalIndex[1] - 1];
                vertices.push_back(vert2);

                Vertex vert3;
                vert3.pos = temp_verts[vertexIndex[2] - 1];
                vert3.pos.w = 1;
                vert3.uv[0] = temp_uvs[uvIndex[2] - 1].x;
                vert3.uv[1] = temp_uvs[uvIndex[2] - 1].y;
                vert3.normal = temp_normals[normalIndex[2] - 1];
                vertices.push_back(vert3);
                if(matches == 12){
                    vertexAmount += 3;

                    Vertex vert4;
                    vert4.pos = temp_verts[vertexIndex[0] - 1];
                    vert4.pos.w = 1;
                    vert4.uv[0] = temp_uvs[uvIndex[0] - 1].x;
                    vert4.uv[1] = temp_uvs[uvIndex[0] - 1].y;
                    vert4.normal = temp_normals[normalIndex[1] - 1];
                    vertices.push_back(vert4);

                    Vertex vert5;
                    vert5.pos = temp_verts[vertexIndex[2] - 1];
                    vert5.pos.w = 1;
                    vert5.uv[0] = temp_uvs[uvIndex[2] - 1].x;
                    vert5.uv[1] = temp_uvs[uvIndex[2] - 1].y;
                    vert5.normal = temp_normals[normalIndex[2] - 1];
                    vertices.push_back(vert5);

                    Vertex vert6;
                    vert6.pos = temp_verts[vertexIndex[3] - 1];
                    vert6.pos.w = 1;
                    vert6.uv[0] = temp_uvs[uvIndex[3] - 1].x;
                    vert6.uv[1] = temp_uvs[uvIndex[3] - 1].y;
                    vert6.normal = temp_normals[normalIndex[3] - 1];
                    vertices.push_back(vert6);
                }
            }else{
                std::cout << "ERROR: parser too weak, check MeshResource.cc" << std::endl;
                return nullptr;
            }
        }
    }

    for(GLuint i = 0; i < vertices.size(); i+=3){
        indices.push_back(Index{i, i+1, i+2});
    }
    std::cout << "Successfully created indices" << std::endl;

    Vertex* verts = new Vertex[vertices.size()];
    for(int i = 0; i < vertices.size(); i++){
        verts[i] = vertices[i];
        //std::cout << "----VERTEX" << i << "----" << std::endl;
        //verts[i].pos.Print();
        //std::cout << "  --UV--  " << std::endl;
        //std::cout << verts[i].uv[0] << ", " << verts[i].uv[1] << std::endl;
        //std::cout << "  --NORMAL--  " << std::endl;
        //verts[i].normal.Print();
        //std::cout << endl;
    }
    Index* inds = new Index[indices.size()];
    for(int i = 0; i < indices.size(); i++){
        inds[i] = indices[i];
        //std::cout << "----INDEX" << i << "----" << std::endl;
        //std::cout << inds[i].v1 << ", " << inds[i].v2 << ", " << inds[i].v3 << endl;
        //std::cout << endl;
    }
    std::cout << "Commencing upload to GPU" << std::endl;
    objMesh->UploadToGPU(verts, vertices.size(), inds, indices.size());
    std::cout << "Successful upload to GPU" << std::endl;
    delete(verts);
    delete(inds);
    return objMesh;
}


std::shared_ptr<MeshResource> MeshResource::PointLightCube(Vector color){

    std::shared_ptr<MeshResource> light(new MeshResource());
    float size = 0.1;
	Vertex vertices[24] = {	
		// Front
		Vertex{Vector(-size, -size, size), color, {0.5, 0.75}},
		Vertex{Vector(-size, size, size), color, {0.75, 0.75}},
		Vertex{Vector(size, -size, size), color, {0.5, 1.0}},
		Vertex{Vector(size, size, size), color, {0.75, 1.0}},

		// Left
		Vertex{Vector(-size, -size, size), color, {0.25, 0.5}},
		Vertex{Vector(-size, size, size), color, {0.5, 0.5}},
		Vertex{Vector(-size, -size, -size), color, {0.25, 0.75}},
		Vertex{Vector(-size, size, -size), color, {0.5, 0.75}},

		// Right
		Vertex{Vector(size, -size, -size), color, {0.75, 0.5}},
		Vertex{Vector(size, size, -size), color, {1.0, 0.5}},
		Vertex{Vector(size, -size, size), color, {0.75, 0.75}},
		Vertex{Vector(size, size, size), color, {1.0, 0.75}},

		// Over
		Vertex{Vector(-size, size, -size), color, {0.5, 0.5}},
		Vertex{Vector(-size, size, size), color, {0.75, 0.5}},
		Vertex{Vector(size, size, -size), color, {0.5, 0.75}},
		Vertex{Vector(size, size, size), color, {0.75, 0.75}},

		// Under
		Vertex{Vector(-size, -size, size), color, {0.0, 0.5}},
		Vertex{Vector(-size, -size, -size), color, {0.25, 0.5}},
		Vertex{Vector(size, -size, size), color, {0.0, 0.75}},
		Vertex{Vector(size, -size, -size), color, {0.25, 0.75}},

		// Back
		Vertex{Vector(-size, -size, -size), color, {0.5, 0.25}},
		Vertex{Vector(-size, size, -size), color, {0.75, 0.25}},
		Vertex{Vector(size, -size, -size), color, {0.5, 0.5}},
		Vertex{Vector(size, size, -size), color, {0.75, 0.5}}
		};

	Index indices[12] = {
		Index{0,1,3},
		Index{0,2,3},

		Index{4,5,7},
		Index{4,6,7},
		
		Index{8,9,11},
		Index{8,10,11},
		
		Index{12,13,15},
		Index{12,14,15},
		
		Index{16,17,19},
		Index{16,18,19},
		
		Index{20,21,23},
		Index{20,22,23}
	};
	light->UploadToGPU(vertices, 24, indices, 12);
    return light;
}
