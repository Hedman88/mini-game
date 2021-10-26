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
private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;
    Score scoreUI;
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
