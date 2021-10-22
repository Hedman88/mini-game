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
#include "GLFW/glfw3.h"

//mini_game
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <ctime>
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

    window->SetMouseMoveFunction([this](float64 x, float64 y)
    {
        // std::cout << x << " " << y << std::endl;
        // float aimAngle;
        // if (x <= windowWidth && y <= windowHeight)
        //     aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y)) - M_PI;
        // else if (x <= windowWidth && y >= windowHeight)
        //     aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y));
        // else if (x >= windowWidth && y >= windowHeight)
        //     aimAngle = atanf(float(x - windowWidth) / float(y - windowHeight));
        // else if (x >= windowWidth && y <= windowHeight)
        //     aimAngle = atanf(float(windowWidth - x) / float(windowHeight - y)) - M_PI;

        // this->mouseRot = aimAngle;
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
    const char* objPath = "assets/models/Wolf.obj";
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
    
    // The additive position vector for the model
    Vector modelPos = Vector(0,0,0,1);
    // How fast will the cube move?

    const int shootingRate = 1; // coolDown effect
    int shootingTimer; // in seconds currently
    
    //stores the players direction and position when LMBPressed = true
    Matrix firingRotation;
    Vector bulletTrailStart;
    
    camera.SetRot(RotationX(M_PI / 2.f));
    window->GetSize(windowWidth, windowHeight);
    windowWidth >>= 1;
    windowHeight >>= 1;

    Player pl;
    pl.position = modelPos;
    pl.radius = 0.5f;

    unsigned int waves = 1;
    std::vector<Enemy> enemies;

    Enemy::SpawnEnemies(&enemies, waves, 40, 30);

    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].graphicNode->SetMR(MeshResource::LoadObj(objPath));
        enemies[i].graphicNode->InitNode(vsPath, psPath, texturePath);
        enemies[i].graphicNode->SetSR(gNode.GetSR());
    }


	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

        GLFWgamepadstate state;
        if(glfwGetGamepadState(GLFW_JOYSTICK_1, &state)){
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f)
            {
                // firing at enenmy
            }
            // map.tiles;
            //left is -1
            //up is -1
            //down is 1
            //right is 1
            const float CONTROLLER_DEADZONE = 0.1f;

            //check if the axis input is significant

            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > CONTROLLER_DEADZONE ||
                state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -CONTROLLER_DEADZONE)
            {
                //if (player->position + player->radius < setOfTiles[player->position].size / 2)
                modelPos.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] * pl.moveSpeed;
            }

            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > CONTROLLER_DEADZONE ||
                state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -CONTROLLER_DEADZONE)
            {
                // test if a wall is along the forward vector
                //if (player->position + setOfTiles[player->position].size / 2)
                    modelPos.z += state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] * pl.moveSpeed;
            }

            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] != 0.f)
            {
                if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.f)
                    this->mouseRot = atanf(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] / state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
                else if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y < -0.f])
                    this->mouseRot = atanf(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] / state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]) - M_PI;
                
            }

            pl.position = modelPos;
        }
        
        for (size_t i = 0; i < enemies.size(); i++)
        {
            if ((pl.position - enemies[i].position).Length() < pl.radius)
            {
                printf("GAME OVER!\n");
                this->window->Close();
                //enemies[i].position = Vector(0, 0, -700);
                //waves++;
            }
            else
            {
                enemies[i].Update(pl);
            }
            
        }
        
        camera.SetPos(modelPos * -1.f);

        // Vector moveInput(this->right - this->left, 0, this->down - this->up);
        // if (moveInput.Length())
        //     moveInput.Normalize();
        // moveInput = moveInput * moveSpeed;
        // modelPos = modelPos + moveInput;

        // The light node sends up its values to the meshes shader program
        lightNode.GiveLight(camera.GetPos());
        
        gNode.Draw(camera.GetVPMatrix(), PositionMat(modelPos) * RotationY(mouseRot));

        lightNode.Draw(camera.GetVPMatrix());

        for (size_t i = 0; i < enemies.size(); i++)
        {
            enemies[i].graphicNode->Draw(camera.GetVPMatrix(), PositionMat(enemies[i].position) * firingRotation);
        }

        // bulletNode.Draw(camera.GetVPMatrix(), PositionMat(en.position) * firingRotation);
        map.Draw(camera.GetVPMatrix());

        lightNode.Draw(camera.GetVPMatrix() * RotationY(0.01*i));
        i++;

		this->window->SwapBuffers();
	}
}

} // namespace Example
