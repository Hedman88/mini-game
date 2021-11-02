#pragma once
#include "config.h"
#include "render/window.h"

class Score
{
    int score = 0;
    int highScore = 0;
    int fps = -1;
    bool dead = false;

  public:
    void Render();
    void IncrementScore();
    void UploadFPS(float fps);
    void SaveScore();
    void LoadScore();
    void SetGameOverScreen(bool activated);
    void ToggleGameOverScreen();
    bool GetDead();
    void Reset();
};
