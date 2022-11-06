//
//  Utils.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/09/19.
//

#ifndef Utils_h
#define Utils_h

#include <tuple>
#include <iostream>
#include <cmath>
#include <random>
#include <limits>
#include <memory>

#include "Vec3.hpp"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double Infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) {
  return degrees * Pi / 180.0;
}

inline double clamp(double x, double min, double max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

static void writeColor(std::ostream &out, Color pixelColor, double SamplesPerPixel) {
  auto r = pixelColor.x();
  auto g = pixelColor.y();
  auto b = pixelColor.z();
  
  // Divide color by the number of samples and gamma-correct for gamma=2.0
  auto scale = 1.0 / SamplesPerPixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);
  
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif /* Utils_h */

