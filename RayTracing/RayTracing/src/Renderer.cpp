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

void Renderer::Render(const Scene &scene, const Camera &camera) {
  Ray ray;
  ray.Origin = camera.GetPosition();

  uint32_t height = m_FinalImage->GetHeight();
  uint32_t width = m_FinalImage->GetWidth();
  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      ray.Direction = camera.GetRayDirections()[x + y * width];
      glm::vec4 color = TraceRay(scene, ray);
      color = glm::clamp(color, glm::vec4(0.f), glm::vec4(1.f));
      m_ImageData[x + y * width] = Utils::ConvertToRGBA(color);
    }
  }

  m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Scene &scene, const Ray &ray) {

  /*
   * (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -
   * r^2) = 0 where a = ray origin b = ray direction r = sphere radius t = hit
   * distance
   */

  // Check spheres
  glm::vec4 clearColor{0, 0, 0, 1};
  if (!scene.Spheres.size())
    return clearColor;

  // Check if ray hits spheres
  const Sphere *closestSphere = nullptr;
  float hitDistance = std::numeric_limits<float>::max();
  for (const Sphere &sphere : scene.Spheres) {
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
    if (closestT < hitDistance) {
      hitDistance = closestT;
      closestSphere = &sphere;
    }
  }

  if (closestSphere == nullptr)
    return clearColor;

  // Translate back to the origin
  glm::vec3 origin = ray.Origin - closestSphere->Position;

  // Hit coords: p = a + bt
  // Not the same a and b from quadratic equation, but from the original
  // equation
  glm::vec3 hitPoint = origin + ray.Direction * hitDistance;
  glm::vec3 normal = glm::normalize(hitPoint);

  // Light direction and angle between light and pixel normal.
  glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
  float d = glm::max(glm::dot(normal, -lightDir), 0.f); // == cos(angle)

  // Assign color to the sphere
  glm::vec3 sphereColor = closestSphere->Albedo;
  sphereColor = sphereColor * d;
  return glm::vec4(sphereColor, 1.f);
}