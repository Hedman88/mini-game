#include "config.h"
#include "Debug.h"
#include "GL/glew.h"
#include "render/ShaderResource.h"
#define MAX_SIZE 8192*4
namespace Debug{
    struct Vertex
    {
        Vector pos;
        Vector color;
    };
    struct DebugRenderBuffer
    {
        GLuint vbo = 0;
        int verticesAmount = 0;
        Vertex vertices[MAX_SIZE];
        ShaderResource sr;
    };

    DebugRenderBuffer triBuf;
    DebugRenderBuffer lineBuf;

    inline void PushVertex(DebugRenderBuffer *buf, Vertex vert){
        if(buf->verticesAmount >= MAX_SIZE){
            return;
        }
        buf->vertices[buf->verticesAmount] = vert;
        buf->verticesAmount++;
    }

    void DrawSquare(float size, Vector pos, Vector color){
        PushVertex(&triBuf, Vertex{pos + Vector(-size/2, 0, -size/2), color});
        PushVertex(&triBuf, Vertex{pos + Vector(-size/2, 0, size/2), color});
        PushVertex(&triBuf, Vertex{pos + Vector(size/2, 0, size/2), color});

        PushVertex(&triBuf, Vertex{pos + Vector(-size/2, 0, -size/2), color});
        PushVertex(&triBuf, Vertex{pos + Vector(size/2, 0, -size/2), color});
        PushVertex(&triBuf, Vertex{pos + Vector(size/2, 0, size/2), color});
    }

    void DrawLine(Vector start, Vector end, Vector color){
        PushVertex(&lineBuf, Vertex{start, color});
        PushVertex(&lineBuf, Vertex{end, color});
    }

    void Render(Matrix cameraVPMatrix){
        //Rendering debug triangles
        if(triBuf.vbo == 0){
            triBuf.sr.LoadShaders("engine/render/DebugVShader.glsl", "engine/render/DebugPShader.glsl");
            glGenBuffers(1, &triBuf.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, triBuf.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_SIZE, nullptr, GL_DYNAMIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, triBuf.vbo);
        }
        triBuf.sr.ActivateShaders();
        triBuf.sr.SetMatrix("matrix", cameraVPMatrix.data2);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * triBuf.verticesAmount, triBuf.vertices);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
	    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*4));
        glDrawArrays(GL_TRIANGLES, 0, triBuf.verticesAmount);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        triBuf.verticesAmount = 0;

        // Rendering debug lines
        if(lineBuf.vbo == 0){
            lineBuf.sr.LoadShaders("engine/render/DebugVShader.glsl", "engine/render/DebugPShader.glsl");
            glGenBuffers(1, &lineBuf.vbo);
            glBindBuffer(GL_ARRAY_BUFFER, lineBuf.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_SIZE, nullptr, GL_DYNAMIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, lineBuf.vbo);
        }
        lineBuf.sr.ActivateShaders();
        lineBuf.sr.SetMatrix("matrix", cameraVPMatrix.data2);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * lineBuf.verticesAmount, lineBuf.vertices);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
	    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*4));
        glDrawArrays(GL_LINES, 0, lineBuf.verticesAmount);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        triBuf.verticesAmount = 0;
    }
}
