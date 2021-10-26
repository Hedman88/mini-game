#include "Score.h"
#include "imgui.h"

void Score::InitUI(Display::Window *window){
	// stahp
}
void Score::Render(Display::Window *window){
    if(window->Open()){
        ImGui::Begin("Testing");
        ImGui::Text("Hello, world!");
        ImGui::End();
    }
}
void Score::Cleanup(){
    // FIXED: Imgui setup and cleanup is handled in window.cc
}
void Score::IncrementScore(){

}
void Score::SaveScore(){

}
void Score::LoadScore(){

}
