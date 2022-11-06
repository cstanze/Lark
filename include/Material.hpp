//
//  Material.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/11/5.
//

#ifndef Material_h
#define Material_h

#include "Ray.hpp"
#include "Vec3.hpp"

struct HitRecord;

class Material {
public:
  virtual ~Material() = default;

  virtual bool scatter(
    const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered
  ) const = 0;
};

#endif
