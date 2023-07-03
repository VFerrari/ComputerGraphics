#pragma once

#include <vector>

#include <glm/glm.hpp>

struct Material {
  glm::vec3 Albedo{1.f};
  float Roughness = 1.f;
  float Metallic = 0.f;
  float EmissionPower = 0.f;
  glm::vec3 EmissionColor{0.f};

  glm::vec3 getEmission() const { return EmissionPower * EmissionColor; }
};

struct Sphere {
  glm::vec3 Position{0.f};
  float Radius = 0.5f;

  int MaterialIdx = 0;
};

struct Scene {
  std::vector<Sphere> Spheres;
  std::vector<Material> Materials;
};