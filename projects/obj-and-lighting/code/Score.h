#pragma once
#include "config.h"
#include "render/window.h"

class Score
{
    int score = 0;
    int highScore;

  public:
    void InitUI(Display::Window *window);
    void Render(Display::Window *window);
    void Cleanup();
    void IncrementScore();
    void SaveScore();
    void LoadScore();
};
