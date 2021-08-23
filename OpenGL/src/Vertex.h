#pragma once

#include <glm/glm.hpp>

class Vertex {
 public:
  glm::vec2 Position;
  glm::vec4 Color;
  glm::vec2 TexCoords;
  float TexID;

 public:
  Vertex() {}
  Vertex(glm::vec2 pos, glm::vec4 color, glm::vec2 texCoords, float texID)
      : Position(pos), Color(color), TexCoords(texCoords), TexID(texID) {}
};