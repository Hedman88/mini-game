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
#include "imgui.h"
#include "render/Debug.h"

//Linux specific
#ifdef __linux__
#include <unistd.h>
#endif

//mini_game
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <ctime>
#include <chrono>
#include "entity.h"
#include "enemy.h"
#include "player.h"
#include "Ray.h"

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
        if(key == GLFW_KEY_B && action == GLFW_PRESS){
            map.ToggleDebugMode();
        }
	});
    window->SetMousePressFunction([this](int32 button, int32 action, int32 mods){
        if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
            this->LMBPressed = true;
            this->scoreUI.IncrementScore();
        }else if(action == GLFW_RELEASE){
            this->LMBPressed = false;
        }
        if(button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS){
            this->scoreUI.ToggleGameOverScreen();
        }
    });

    window->SetMouseMoveFunction([this](float64 x, float64 y)
    {
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

        // set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->scoreUI.Render();
		});
        this->scoreUI.LoadScore();

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

    // Player
    const char* vsPath = "engine/render/VertexShader.ascii";
    const char* psPath = "engine/render/PixelShader.ascii";
    const char* texturePath = "assets/textures/grey.png";
    const char* objPath = "assets/models/Wolf.obj";
    GraphicsNode gNode(objPath);
    gNode.InitNode(vsPath, psPath, texturePath);

    // Map and tiles
    const char* roadTexture = "assets/kenney_retroUrbanKit/Models/OBJ_format/Textures/grass.png";
    const char* wallTexture = "assets/kenney_retroUrbanKit/Models/OBJ_format/Textures/wall.png";
    map.GenerateMap(10, 2);
    map.InitTiles(vsPath, psPath, roadTexture, wallTexture);
    
    
    // Lights
    const char* lvsPath = "engine/render/PointLightVS.ascii";
    const char* lpsPath = "engine/render/PointLightPS.ascii";
    PointLightNode lightNode(Vector(0,1,0), Vector(1,1,1,1), 1);
    lightNode.InitNode(lvsPath, lpsPath);
    lightNode.SetSharedShader(gNode.GetSR());

    // Enemies
    std::shared_ptr<GraphicsNode> gNodeEnemy = std::make_shared<GraphicsNode>(objPath);
    gNodeEnemy->InitNode(vsPath, psPath, texturePath);


	int width, height;
	window->GetSize(width, height);
	Camera camera = Camera(90, width, height, 0.001, 1000);
    
    //stores the players direction and position when LMBPressed = true
    Matrix firingRotation;
    Vector bulletTrailStart;
    
    camera.SetRot(RotationX(M_PI / 2.f));
    window->GetSize(windowWidth, windowHeight);

    Player pl;
    pl.position = Vector(8,0,8,1);
    pl.radius = 0.2f;
    pl.gNode = &gNode;
    //for (size_t i = 0; i < 16 * 16; i++)
    //{
    //    // if (map.GetTile(i % 16, i / 16)->Walkable)
    //    map.GetTile(i % 16, i / 16)->gNode->SetSR(pl.gNode->GetSR());
    //}
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            map.GetTile(x, y)->gNode->SetSR(pl.gNode->GetSR());
        }
    }

    const float CONTROLLER_DEADZONE = 0.1f;

    unsigned int waves = 1;
    std::vector<Enemy> enemies;
    Enemy::SpawnEnemies(&enemies, &map, waves, 16, 16);
    for (size_t i = 0; i < enemies.size(); i++)
    {
        enemies[i].SetGNode(gNodeEnemy);
        enemies[i].ID = i;
        enemies[i].score = &scoreUI;
    }

	while (this->window->IsOpen())
	{
        auto start = std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

        GLFWgamepadstate state;
        if (!scoreUI.GetDead())
        {
            if(glfwGetGamepadState(GLFW_JOYSTICK_1, &state)){
                if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
                {
                    this->window->Close();
                }
                if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])
                {
                    Reset(&pl, &waves, &enemies, gNodeEnemy, &map, &scoreUI);
                }
                if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -0.5f && !scoreUI.GetDead())
                {
                    // firing at enemy
                    pl.Shoot(&map, &enemies);
                }

                //check if the axis input is significant
                //right
                if ((state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > CONTROLLER_DEADZONE &&
                    map.GetTile(int(pl.position.x + pl.radius / 2), int(pl.position.z))->walkable) && !scoreUI.GetDead())
                {   
            {   
                {   
            {   
                {   
                    pl.position.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] * pl.moveSpeed;
                }

                //left
                if ((state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -CONTROLLER_DEADZONE &&
                    map.GetTile(int(pl.position.x - pl.radius), int(pl.position.z))->walkable) && !scoreUI.GetDead())
                {
                    pl.position.x += state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] * pl.moveSpeed;
                }

                // down
                if ((state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > CONTROLLER_DEADZONE &&
                    map.GetTile(int(pl.position.x), int(pl.position.z + pl.radius / 2))->walkable) && !scoreUI.GetDead())
                {   
            {   
                {   
            {   
                {   
                    pl.position.z += state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] * pl.moveSpeed;
                }
            
                //up
                if ((state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -CONTROLLER_DEADZONE &&
                    map.GetTile(int(pl.position.x), int(pl.position.z - pl.radius))->walkable) && !scoreUI.GetDead())
                {
                    pl.position.z += state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] * pl.moveSpeed;
                }

                if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] != 0.f && !scoreUI.GetDead())
                {
                    if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.f){
                        pl.aimDir = Vector(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], 0, state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
                        pl.rotation = atanf(pl.aimDir.x / pl.aimDir.z);
                    }else if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -0.f){
                        pl.aimDir = Vector(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], 0, state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
                        pl.rotation = atanf(pl.aimDir.x / pl.aimDir.z) - M_PI;
                    }
                }
            }
        
            else  {
                Vector moveInput(this->right - this->left, 0, this->down - this->up);
                if (moveInput.Length())
                    moveInput.Normalize();
                moveInput = moveInput * pl.moveSpeed;
                pl.position = pl.position + moveInput;
            }
        }


        for (size_t i = 0; i < enemies.size(); i++)
        {
            if (!scoreUI.GetDead() && enemies.size() <= GetDeadAmount())
            {
                waves++;
                Enemy::SpawnEnemies(&enemies, &map, waves, 16, 16);
                for (size_t i = 0; i < enemies.size(); i++)
                {
                    enemies[i].SetGNode(gNodeEnemy);
                    enemies[i].ID = i;
                    enemies[i].score = &scoreUI;
                    SetDeadAmount();
                }
            }
            if(enemies[i].dead){
                continue;
            }            
            if ((pl.position - enemies[i].position).Length() < pl.radius)
            {
                scoreUI.SetGameOverScreen(true);
            }
            else
            {
                enemies[i].Update(pl, &map);
            }
        }
        
        camera.SetPos(pl.position * -1.f);


        // The light node sends up its values to the meshes shader program
        
        pl.gNode->Draw(camera.GetVPMatrix(), PositionMat(pl.position) * RotationY(pl.rotation));

        // Draw the Enemies
        for (size_t i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].dead) continue;
			if (float(pl.position.z - enemies[i].position.z) > 0.f)
            {
                enemies[i].graphicNode->Draw(
                    camera.GetVPMatrix(),
                    PositionMat(enemies[i].position) *
                    RotationY(
                        atanf(
                            float(pl.position.x - enemies[i].position.x) /
                            float(pl.position.z - enemies[i].position.z))
                            )
                        );
            }
            
            if (float(pl.position.z - enemies[i].position.z) < -0.f)
            {
                enemies[i].graphicNode->Draw(
                    camera.GetVPMatrix(),
                    PositionMat(enemies[i].position) *
                    RotationY(
                        atanf(
                            float(pl.position.x - enemies[i].position.x) /
                            float(pl.position.z - enemies[i].position.z))
                             - M_PI
                            )
                        );
            }
        }

        map.Draw(camera.GetVPMatrix());
        lightNode.SetPos(Vector(pl.position.x, 1, pl.position.z));
        // Generating some light for shots
        if(pl.shotFired){
            lightNode.GiveLight(camera.GetPos());
            pl.shotFired = false;
        }else{
            lightNode.StopGivingLight();
        }
        //lightNode.Draw(camera.GetVPMatrix(), pl.rotation);
		
        Debug::Render(camera.GetVPMatrix());
		this->window->SwapBuffers();

        // Calculate framerate
        std::chrono::nanoseconds elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        this->scoreUI.UploadFPS(1000000/(float)microseconds);
	}
}
void
ExampleApp::Reset(Player* pl, unsigned int* waves, std::vector<Enemy>* enemies, std::shared_ptr<GraphicsNode> gNodeEnemy, Map* map, Score* scoreUI)
{
    // Reset player
    pl->position = Vector(8,0,8,1);

    // Reset enemies and waves
    *waves = 1;
    Enemy::SpawnEnemies(enemies, map, *waves, 16, 16);
    for (size_t i = 0; i < enemies->size(); i++)
    {
        (*enemies)[i].SetGNode(gNodeEnemy);
        (*enemies)[i].ID = i;
        (*enemies)[i].score = scoreUI;
    }
    
    // Reset map
    const char* vsPath = "engine/render/VertexShader.ascii";
    const char* psPath = "engine/render/PixelShader.ascii";
    const char* roadTexture = "assets/kenney_retroUrbanKit/Models/OBJ_format/Textures/grass.png";
    const char* wallTexture = "assets/kenney_retroUrbanKit/Models/OBJ_format/Textures/wall.png";
    map->GenerateMap(10, 2);
    map->InitTiles(vsPath, psPath, roadTexture, wallTexture);
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            map->GetTile(x, y)->gNode->SetSR(pl->gNode->GetSR());
        }
    }

    // Reset score
    scoreUI->Reset();
}
void
ExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
        //ImGui::Begin();
		/*
		// Example for projecting text from worldspace into screenspace
		Math::vec4 vec = Vector(0, 1, -2, 1);
		// transform point into canonical view volume (normalized device coordinates)
		Math::vec4 ndc = cam->viewProjection * vec;
		// perspective divide
		ndc /= Math::vec4(ndc.w);

		if (ndc.z <= 1) // only render in front of camera
		{
			Math::vec2 cursorPos = { ndc.x, ndc.y };
			// Move cursor pos into screenspace coordinates (0 -> screen width , etc.)
			cursorPos += {1.0f, 1.0f};
			cursorPos *= 0.5f;
			cursorPos.y = 1.0f - cursorPos.y; // invert y axis
			cursorPos.x *= ImGui::GetWindowWidth();
			cursorPos.y *= ImGui::GetWindowHeight();
			cursorPos.x -= ImGui::CalcTextSize(cmd.text.AsCharPtr()).x / 2.0f; // center text
			ImGui::SetCursorPos({ cursorPos.x, cursorPos.y });
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,0.4,0.3,1));
			ImGui::TextUnformatted("Hello world");
			ImGui::PopStyleColor();
		}
		*/

		//ImGui::End();
	}
}
} // namespace Example
