export module math.colour;

import <span>;

export struct ColourRGBA {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 0.0f;

  ColourRGBA() = default;
  explicit ColourRGBA(float r, float g, float b, float a = 1.0f)
  : r(r), g(g), b(b), a(a) {}

  void Set(std::span<const float, 4> elts)
  {
	  r = elts[0]; g = elts[1]; b = elts[2]; a = elts[3];
  }
 };