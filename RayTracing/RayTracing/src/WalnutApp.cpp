#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Camera.h"
#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer {
public:
  ExampleLayer() : m_Camera(45.f, 0.1f, 100.f) {

    Material &pinkSphere = m_Scene.Materials.emplace_back();
    pinkSphere.Albedo = {1.f, 0.f, 1.f};
    pinkSphere.Roughness = 0.f;

    Material &blueSphere = m_Scene.Materials.emplace_back();
    blueSphere.Albedo = {0.2f, 0.3f, 1.f};
    blueSphere.Roughness = 0.1f;

    {
      Sphere sphere{.Radius = 1.f, .MaterialIdx = 0};
      m_Scene.Spheres.push_back(sphere);
    }
    {
      Sphere sphere{
          .Position = {0.f, -101.f, 0.f}, .Radius = 100.f, .MaterialIdx = 1};
      m_Scene.Spheres.push_back(sphere);
    }
  }

  virtual void OnUpdate(float ts) override {
    if (m_Camera.OnUpdate(ts)) {
      m_Renderer.ResetFrameIdx();
    }
  }

  virtual void OnUIRender() override {
    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3f ms", m_LastRenderTime);
    if (ImGui::Button("Render")) {
      Render();
    }

    ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);

    if (ImGui::Button("Reset")) {
      m_Renderer.ResetFrameIdx();
    }
    ImGui::End();

    ImGui::Begin("Scene");
    for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
      ImGui::PushID(i);

      Sphere &sphere = m_Scene.Spheres[i];
      ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
      ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
      ImGui::DragInt("Material", &sphere.MaterialIdx, 1.f, 0.f,
                     (int)m_Scene.Materials.size() - 1);

      ImGui::Separator();

      ImGui::PopID();
    }

    for (size_t i = 0; i < m_Scene.Materials.size(); i++) {
      ImGui::PushID(i);

      Material &material = m_Scene.Materials[i];
      ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
      ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.f, 1.f);
      ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.f, 1.f);

      ImGui::Separator();

      ImGui::PopID();
    }

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    m_ViewportWidth = ImGui::GetContentRegionAvail().x;
    m_ViewportHeight = ImGui::GetContentRegionAvail().y;

    auto image = m_Renderer.GetFinalImage();
    if (image) {
      ImGui::Image(image->GetDescriptorSet(),
                   {(float)image->GetWidth(), (float)image->GetHeight()},
                   ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
    ImGui::PopStyleVar();

    Render();
  }

  void Render() {
    Timer timer;

    m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
    m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
    m_Renderer.Render(m_Scene, m_Camera);

    m_LastRenderTime = timer.ElapsedMillis();
  }

private:
  Camera m_Camera;
  Renderer m_Renderer;
  Scene m_Scene;
  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

  float m_LastRenderTime = 0.0f;
};

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
  Walnut::ApplicationSpecification spec;
  spec.Name = "Ray Tracing";

  Walnut::Application *app = new Walnut::Application(spec);
  app->PushLayer<ExampleLayer>();
  app->SetMenubarCallback([app]() {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        app->Close();
      }
      ImGui::EndMenu();
    }
  });
  return app;
}
