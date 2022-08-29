#pragma once

// STL
#include <memory>

// GLM
#include <glm/glm.hpp>

// Walnut
#include "Walnut/Image.h"

// App
#include "Camera.h"
#include "Ray.h"

class Renderer {
public:
  Renderer() = default;

  void OnResize(uint32_t width, uint32_t height);
  void Render(const Camera &camera);

  std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

private:
  glm::vec4 TraceRay(const Ray &ray);

private:
  std::shared_ptr<Walnut::Image> m_FinalImage;
  uint32_t *m_ImageData = nullptr;
};