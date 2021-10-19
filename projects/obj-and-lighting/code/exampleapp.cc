//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "render/MeshResource.h"
#include "render/TextureResource.h"
#include "render/ShaderResource.h"
#include "render/GraphicsNode.h"
#include "render/PointLightNode.h"
#include "render/Camera.h"
#include "core/mathLib.h"
#include "render/stb_image.h"

//mini_game
#include <cmath>
#include "entity.h"
#include "enemy.h"
#include "player.h"

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec4 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 uvIn;\n"
"layout(location=0) out vec4 Color;\n"
"layout(location=1) out vec2 uvOut;\n"
"uniform mat4 matrix;\n"
"void main()\n"
"{\n"
"	gl_Position = matrix * pos;\n"
"	Color = color;\n"
"   uvOut = uvIn;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"layout(location=1) in vec2 uvIn;\n"
"out vec4 Color;\n"
"uniform sampler2D image;\n"
"void main()\n"
"{\n"
"	Color = texture(image, uvIn);\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
	{
        if(key == GLFW_KEY_ESCAPE){
            this->window->Close();
        }
        if(key == GLFW_KEY_W && action == GLFW_PRESS){
            this->up = true;
        }else if(key == GLFW_KEY_W && action == GLFW_RELEASE){
            this->up = false;
        }
        if(key == GLFW_KEY_A && action == GLFW_PRESS){
            this->left = true;
        }else if(key == GLFW_KEY_A && action == GLFW_RELEASE){
            this->left = false;
        }
        if(key == GLFW_KEY_S && action == GLFW_PRESS){
            this->down = true;
        }else if(key == GLFW_KEY_S && action == GLFW_RELEASE){
            this->down = false;
        }
        if(key == GLFW_KEY_D && action == GLFW_PRESS){
            this->right = true;
        }else if(key == GLFW_KEY_D && action == GLFW_RELEASE){
            this->right = false;
        }
        if(key == GLFW_KEY_Q && action == GLFW_PRESS){
            this->qPressed = true;
        }else if(key == GLFW_KEY_Q && action == GLFW_RELEASE){
            this->qPressed = false;
        }
        if(key == GLFW_KEY_E && action == GLFW_PRESS){
            this->ePressed = true;
        }else if(key == GLFW_KEY_E && action == GLFW_RELEASE){
            this->ePressed = false;
        }
	});
    window->SetMousePressFunction([this](int32 button, int32 action, int32 mods){
        if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
            this->LMBPressed = true;
        }else if(action == GLFW_RELEASE){
            this->LMBPressed = false;
        }
    });

    window->SetMouseMoveFunction([this](float64 x, float64 y)
    {
        float aimAngle;
        if (x <= windowWidth && y <= windowHeight)
            aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y)) - M_PI;
        else if (x <= windowWidth && y >= windowHeight)
            aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y));
        else if (x >= windowWidth && y >= windowHeight)
            aimAngle = atanf(float(x - windowWidth) / float(y - windowHeight));
        else if (x >= windowWidth && y <= windowHeight)
            aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y)) - M_PI;

        this->mouseRot = aimAngle;
    });

	GLfloat buf[] =
	{
		-0.5f,	-0.5f,	-1,			// pos 0
		1,		0,		0,		1,	// color 0
		0,		0.5f,	-1,			// pos 1
		0,		1,		0,		1,	// color 0
		0.5f,	-0.5f,	-1,			// pos 2
		0,		0,		1,		1	// color 0
	};

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

    const char* vsPath = "../../../engine/render/VertexShader.ascii";
    const char* psPath = "../../../engine/render/PixelShader.ascii";
    const char* texturePath = "../../../assets/textures/grey.png";
    const char* objPath = "../../../assets/models/Wolf.obj";
    GraphicsNode gNode(objPath);
    gNode.InitNode(vsPath, psPath, texturePath);

    const char* lvsPath = "../../../engine/render/PointLightVS.ascii";
    const char* lpsPath = "../../../engine/render/PointLightPS.ascii";
    PointLightNode lightNode(Vector(0,1,0), Vector(1,1,1,1), 1);
    lightNode.InitNode(lvsPath, lpsPath);
    lightNode.SetSharedShader(gNode.GetSR());

	int width, height;
	window->GetSize(width, height);
	Camera camera = Camera(90, width, height, 0.001, 1000);
    // The rotation caused by mouse held in radians
    
    // The additive position vector for the model
    Vector modelPos = Vector(0,0,0,1);
    // How fast will the cube move?
    float moveSpeed = 0.05;
    
    camera.SetRot(RotationX(M_PI / 2.f));
    window->GetSize(windowWidth, windowHeight);
    windowWidth >>= 1;
    windowHeight >>= 1;

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
    
        if(this->up) modelPos.z -= moveSpeed;
        if(this->left) modelPos.x -= moveSpeed;
        if(this->down) modelPos.z += moveSpeed;
        if(this->right) modelPos.x += moveSpeed;

        // The light node sends up its values to the meshes shader program
        lightNode.GiveLight(camera.GetPos());

        camera.SetPos(modelPos);
        gNode.Draw(camera.GetVPMatrix(), PositionMat(modelPos * -1.f) * RotationY(mouseRot));

        lightNode.Draw(camera.GetVPMatrix());

		this->window->SwapBuffers();
	}
}

} // namespace Example
