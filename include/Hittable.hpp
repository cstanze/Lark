//
//  Hittable.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/10/03.
//

#ifndef Hittable_h
#define Hittable_h

#include "Vec3.hpp"
#include "Ray.hpp"
#include "Utils.hpp"
#include "Material.hpp"

struct HitRecord {
  Point3 p;
  Vec3 normal;
  shared_ptr<Material> matPtr;
  double t;
  bool frontFace;
  
  inline void setFaceNormal(const Ray &r, const Vec3 &outwardNormal) {
    frontFace = dot(r.direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};

#endif /* Hittable_h */
