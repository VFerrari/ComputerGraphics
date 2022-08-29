#include "Renderer.h"

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
}

void Renderer::Render(const Camera &camera) {
  Ray ray;
  ray.Origin = camera.GetPosition();

  uint32_t height = m_FinalImage->GetHeight();
  uint32_t width = m_FinalImage->GetWidth();
  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      ray.Direction = camera.GetRayDirections()[x + y * width];
      glm::vec4 color = TraceRay(ray);
      color = glm::clamp(color, glm::vec4(0.f), glm::vec4(1.f));
      m_ImageData[x + y * width] = Utils::ConvertToRGBA(color);
    }
  }

  m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray &ray) {
  float radius = 0.5f;
  // rayDirection = glm::normalize(rayDirection);

  /*
   * (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -
   * r^2) = 0 where a = ray origin b = ray direction r = sphere radius t = hit
   * distance
   */

  // at^2 + bt + c
  float a = glm::dot(ray.Direction, ray.Direction);
  float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
  float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

  // Delta: Check ray hit
  // b^2 - 4ac
  float delta = b * b - 4.f * a * c;
  if (delta < 0)
    return glm::vec4(0, 0, 0, 1);

  // Solutions: hit distance
  // (-b +- sqrt(delta)) / 2a
  float deltaSqrt = glm::sqrt(delta);
  float closestT = (-b - deltaSqrt) / (2.f * a);
  // float farthestT = (-b + deltaSqrt) / (2.f * a);

  // Hit coords: p = a + bt
  // Not the same a and b from quadratic equation, but from the original
  // equation
  glm::vec3 hitPoint = ray.Origin + ray.Direction * closestT;
  glm::vec3 normal = glm::normalize(hitPoint);

  // Light direction and angle between light and pixel normal.
  glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
  float d = glm::max(glm::dot(normal, -lightDir), 0.f); // == cos(angle)

  // Assign color to the sphere
  glm::vec3 sphereColor(1, 0, 1);
  sphereColor = sphereColor * d;
  return glm::vec4(sphereColor, 1.f);
}