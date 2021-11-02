#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Score.h"
#include "Map.h"

namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();

    void RenderUI();

    void Reset(Player* pl, unsigned int* waves, std::vector<Enemy>* enemies, std::shared_ptr<GraphicsNode> gNodeEnemy, Map* map, Score* scoreUI);
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
    Score scoreUI;
    Map map;
    bool LMBPressed = false;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool qPressed = false;
    bool ePressed = false;

	//mini_game
	float mouseRot;
	int32 windowWidth, windowHeight;
};
} // namespace Example
