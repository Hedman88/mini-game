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
//#include <GLFW/glfw3.h>

//mini_game
#include "entity.h"
#include "enemy.h"
#include "player.h"
#include "Map.h"

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

    const char* vsPath = "engine/render/VertexShader.ascii";
    const char* psPath = "engine/render/PixelShader.ascii";
    const char* texturePath = "assets/textures/grey.png";
    const char* objPath = "assets/kenney_retroUrbanKit/Models/OBJ_format/roadDirt_center.obj";
    GraphicsNode gNode(objPath);
    gNode.InitNode(vsPath, psPath, texturePath);

    Map map;
    map.GenerateMap();
    map.InitTiles(vsPath, psPath, texturePath);
    
    const char* lvsPath = "engine/render/PointLightVS.ascii";
    const char* lpsPath = "engine/render/PointLightPS.ascii";
    PointLightNode lightNode(Vector(0,1,0), Vector(1,1,1,1), 1);
    lightNode.InitNode(lvsPath, lpsPath);
    lightNode.SetSharedShader(gNode.GetSR());

	int width, height;
	window->GetSize(width, height);
	Camera camera = Camera(90, width, height, 0.001, 1000);

    // The rotation caused by mouse held in radians
    float mouseRot = 0.0;
    // The additive position vector for the model
    Vector modelPos = Vector(0,0,0,1);
    // How fast will the cube move?
    float moveSpeed = 0.05;

    int i = 0;

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

        //if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1)){
        //    std::cout << "Gamepad connected" << std::endl;
        //}

        GLFWgamepadstate state;
        if(glfwGetGamepadState(GLFW_JOYSTICK_1, &state)){
            if(state.buttons[GLFW_GAMEPAD_BUTTON_A]){
                std::cout << "A" << std::endl;
            }
            if(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5){
                std::cout << "Trigger" << std::endl;

            }
            if(state.buttons[GLFW_GAMEPAD_BUTTON_BACK]){
                std::cout << "Back" << std::endl;
                //Reset
            }

            modelPos.y -= state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]* moveSpeed;
            modelPos.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] * moveSpeed;
            //player.Move(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_GAMEPAD_AXIS_LEFT_X]);
            //player.Aim(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y, GLFW_GAMEPAD_AXIS_RIGHT_X]);
            //player.Shoot(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]);
        }

		//camera.SetRot(RotationY(0.01 * i));
        if(this->LMBPressed){
            mouseRot += 0.05;
        }
        if(this->qPressed){
		    camera.AddPos(Vector(0, 0, -0.15));
        }
        if(this->ePressed){
		    camera.AddPos(Vector(0, 0, 0.15));
        }

        // WASD = up, down, left, right bools
        if(this->up) modelPos.y += moveSpeed;
        if(this->left) modelPos.x -= moveSpeed;
        if(this->down) modelPos.y -= moveSpeed;
        if(this->right) modelPos.x += moveSpeed;

        // The light node sends up its values to the meshes shader program
        lightNode.GiveLight(camera.GetPos());
        gNode.Draw(camera.GetVPMatrix(), PositionMat(modelPos) * RotationY(mouseRot));

        map.Draw(camera.GetVPMatrix());

        lightNode.Draw(camera.GetVPMatrix() * RotationY(0.01*i));
        i++;

		this->window->SwapBuffers();
	}
}

} // namespace Example
