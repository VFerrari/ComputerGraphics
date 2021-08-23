#include "GUI.h"

// Vendor
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GUI::GUI(GLFWwindow *window) : GUIContext(window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

GUI::~GUI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void GUI::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GUI::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Begin(const char *name) { ImGui::Begin(name); }

void GUI::End() { ImGui::End(); }

void GUI::ShowFramerate() {
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void GUI::ShowDrag2f(const char *name, float *target, float speed) {
  ImGui::DragFloat2(name, target, speed);
}

void GUI::ShowSlider3f(const char *name, float *target, float min, float max) {
  ImGui::SliderFloat3(name, target, min, max);
}

void GUI::ShowColorEdit4(const char *name, float *target) {
  ImGui::ColorEdit4(name, target);
}

bool GUI::Button(const char *name) { return ImGui::Button(name); }