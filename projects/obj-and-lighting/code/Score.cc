#include "Score.h"
#include "imgui.h"
#include <cmath>
#include <iostream>
#include <fstream>

void Score::Render(){
    bool show = true;

	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size, ImGuiCond_Always);
	ImGui::Begin("invis_wnd", &show,
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoNav
		);

	static float x = -2.0f;
	x += 0.005f;
	auto size = ImGui::GetMainViewport()->Size;
	ImGui::SetCursorPos({ 0.03f * size.x, 0.03f * size.y });
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.4, 0.3, 1));
	ImGui::Text("%d", this->score);
	ImGui::SetCursorPos({ 0.03f * size.x, 0.06f * size.y });
    ImGui::Text("%d", this->highScore);
	ImGui::PopStyleColor();

    if(dead){
	    ImGui::SetCursorPos({ 0.46f * size.x, 0.5f * size.y });
        ImGui::Text("YOU DIED");
    }

	ImGui::SetCursorPos({ 0.93f * size.x, 0.97f * size.y });
	ImGui::Text("%d", this->fps);

    ImGui::End();
}

void Score::IncrementScore(){
    this->score++;
    SaveScore();
}

void Score::UploadFPS(float fps){
    this->fps = (int)fps;
}

void Score::SaveScore(){
    if(this->highScore < this->score){
        this->highScore = this->score;
        std::fstream saveFile("high_score.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
        saveFile << this->highScore;
        saveFile.close();
    }
}

void Score::LoadScore(){
    std::ifstream saveFile("high_score.txt");
    std::string line;
    getline(saveFile, line);
    std::cout << line << std::endl;
    std::cout << "WHAT IS GOING ON HERE?!" << std::endl;
    try{
        this->highScore = std::stoi(line);
    }catch(...){
        std::cout << "Failed to load, presumably first time loading" << std::endl;
    }
    saveFile.close();
}

void Score::ToggleGameOverScreen(){
    dead = !dead;
}
