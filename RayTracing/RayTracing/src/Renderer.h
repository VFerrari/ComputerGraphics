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
#include "Scene.h"

class Renderer {
public:
  struct Settings {
    bool Accumulate = true;
    bool SlowRandom = true;
  };

public:
  Renderer() = default;

  void OnResize(uint32_t width, uint32_t height);
  void Render(const Scene &scene, const Camera &camera);

  Settings &GetSettings() { return m_Settings; }
  std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

  void ResetFrameIdx() { m_FrameIdx = 1; }

private:
  struct HitPayload {
    float HitDistance;
    glm::vec3 WorldPosition;
    glm::vec3 WorldNormal;

    int32_t ObjectIdx;
  };

  glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen

  HitPayload TraceRay(const Ray &ray);
  HitPayload ClosestHit(const Ray &ray, float hitDistance, int32_t objectIdx);
  HitPayload Miss(const Ray &ray);

private:
  Settings m_Settings;

  std::shared_ptr<Walnut::Image> m_FinalImage;

  std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

  const Scene *m_ActiveScene = nullptr;
  const Camera *m_ActiveCamera = nullptr;

  uint32_t *m_ImageData = nullptr;
  glm::vec4 *m_AccData = nullptr;

  uint32_t m_FrameIdx = 1;
};