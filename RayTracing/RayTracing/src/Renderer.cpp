#include "Renderer.h"

// STL
#include <cstring>
#include <execution>

// Walnut
#include "Walnut/Random.h"

namespace Utils {
static uint32_t ConvertToRGBA(const glm::vec4 &color) {
  uint8_t r = (uint8_t)(color.r * 255.f);
  uint8_t g = (uint8_t)(color.g * 255.f);
  uint8_t b = (uint8_t)(color.b * 255.f);
  uint8_t a = (uint8_t)(color.a * 255.f);

  return (a << 24) | (b << 16) | (g << 8) | r;
}
} // namespace Utils

void Renderer::OnResize(uint32_t width, uint32_t height) {
  if (m_FinalImage) {

    // No resize necessary
    if (m_FinalImage->GetWidth() == width &&
        m_FinalImage->GetHeight() == height)
      return;

    m_FinalImage->Resize(width, height);
  } else {
    m_FinalImage = std::make_shared<Walnut::Image>(width, height,
                                                   Walnut::ImageFormat::RGBA);
  }

  delete[] m_ImageData;
  m_ImageData = new uint32_t[width * height];

  delete[] m_AccData;
  m_AccData = new glm::vec4[width * height];

  m_ImageHorizontalIter.resize(width);
  m_ImageVerticalIter.resize(height);
  for (uint32_t j = 0; j < width; j++) {
    m_ImageHorizontalIter[j] = j;
  }
  for (uint32_t i = 0; i < height; i++) {
    m_ImageVerticalIter[i] = i;
  }
  ResetFrameIdx();
}

void Renderer::Render(const Scene &scene, const Camera &camera) {
  m_ActiveScene = &scene;
  m_ActiveCamera = &camera;

  uint32_t height = m_FinalImage->GetHeight();
  uint32_t width = m_FinalImage->GetWidth();

  // Initialize acc data
  if (m_FrameIdx == 1) {
    memset(m_AccData, 0, width * height * sizeof(glm::vec4));
  }

  // Draw each pixel
  std::for_each(
      std::execution::par, m_ImageVerticalIter.begin(),
      m_ImageVerticalIter.end(), [this, width](uint32_t y) {
        std::for_each(
            std::execution::par, m_ImageHorizontalIter.begin(),
            m_ImageHorizontalIter.end(), [this, width, y](uint32_t x) {
              // Get frame color and accumulate
              glm::vec4 color = PerPixel(x, y);
              m_AccData[x + y * width] += color;

              // Normalize accumulated color
              glm::vec4 accColor = m_AccData[x + y * width];
              accColor /= (float)m_FrameIdx;

              // Clamp and set color
              accColor = glm::clamp(accColor, glm::vec4(0.f), glm::vec4(1.f));
              m_ImageData[x + y * width] = Utils::ConvertToRGBA(accColor);
            });
      });

  m_FinalImage->SetData(m_ImageData);

  // Accumulate
  if (m_Settings.Accumulate)
    m_FrameIdx++;
  else
    m_FrameIdx = 1;
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y) {
  // Create ray from pixel
  Ray ray;
  ray.Origin = m_ActiveCamera->GetPosition();
  ray.Direction =
      m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

  // Final color
  glm::vec3 light(0.f);
  glm::vec3 contribution(1.f);

  // Bounce the ray N times
  uint8_t bounces = 5;
  for (uint8_t i = 0; i < bounces; i++) {
    // Trace ray and get data
    HitPayload payload = TraceRay(ray);

    // Check if the ray hit something
    if (payload.HitDistance < 0.f) {
      glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
      // light += skyColor * contribution;
      break;
    }

    // Get color from sphere albedo and emission
    const Sphere &sphere = m_ActiveScene->Spheres[payload.ObjectIdx];
    const Material &material = m_ActiveScene->Materials[sphere.MaterialIdx];
    contribution *= material.Albedo;
    light += material.getEmission();

    // Move ray and prepare for next bounce
    ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
    ray.Direction =
        glm::normalize(payload.WorldNormal + Walnut::Random::InUnitSphere());
  }

  return glm::vec4(light, 1.f);
}

Renderer::HitPayload Renderer::TraceRay(const Ray &ray) {

  /*
   * (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -
   * r^2) = 0 where a = ray origin b = ray direction r = sphere radius t = hit
   * distance
   */

  // Check if ray hits spheres
  int32_t closestSphere = -1;
  float hitDistance = std::numeric_limits<float>::max();
  for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++) {
    const Sphere &sphere = m_ActiveScene->Spheres[i];

    // Translate back to the origin
    glm::vec3 origin = ray.Origin - sphere.Position;

    // at^2 + bt + c
    float a = glm::dot(ray.Direction, ray.Direction);
    float b = 2.0f * glm::dot(origin, ray.Direction);
    float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

    // Delta: Check ray hit
    // b^2 - 4ac
    float delta = b * b - 4.f * a * c;
    if (delta < 0)
      continue;

    // Solutions: hit distance
    // (-b +- sqrt(delta)) / 2a
    float deltaSqrt = glm::sqrt(delta);
    // float farthestT = (-b + deltaSqrt) / (2.f * a); // Currently unused
    float closestT = (-b - deltaSqrt) / (2.f * a);

    // Update closest sphere
    if (closestT > 0.f && closestT < hitDistance) {
      hitDistance = closestT;
      closestSphere = (int32_t)i;
    }
  }

  if (closestSphere < 0)
    return Miss(ray);

  return ClosestHit(ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray &ray, float hitDistance,
                                          int32_t objectIdx) {
  HitPayload payload = {.HitDistance = hitDistance, .ObjectIdx = objectIdx};
  const Sphere &closestSphere = m_ActiveScene->Spheres[objectIdx];

  // Translate object to the origin
  glm::vec3 origin = ray.Origin - closestSphere.Position;

  // Hit coords: p = a + bt
  // Not the same a and b from quadratic equation, but from the original
  // equation
  payload.WorldPosition = origin + ray.Direction * hitDistance;
  payload.WorldNormal = glm::normalize(payload.WorldPosition);

  // Translate object back to correct position
  payload.WorldPosition += closestSphere.Position;
  return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray &ray) {
  (void)ray;
  return {.HitDistance = -1.f};
}