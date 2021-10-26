#include "Score.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void Score::InitUI(Display::Window *window){
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfwGL3_Init(window->window, true);
    ImGui::StyleColorsDark();
}
void Score::Render(Display::Window *window){
    if(window->Open()){
        ImGui::Begin("Testing");
        ImGui::Text("Hello, world!");
        ImGui::End();
    }
}
void Score::Cleanup(){
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}
void Score::IncrementScore(){

}
void Score::SaveScore(){

}
void Score::LoadScore(){

}
