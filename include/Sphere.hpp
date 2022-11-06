//
//  Sphere.hpp
//  lark
//
//  Created by Jules Amalie on 2022/10/04.
//

#ifndef Sphere_h
#define Sphere_h

#include "Hittable.hpp"
#include "Vec3.hpp"

class Sphere : public Hittable
{
public:
  Sphere() {}
  Sphere(Point3 center, double r, shared_ptr<Material> m) : center(center), radius(r), matPtr(m) {}

  virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const override;

public:
  Point3 center;
  double radius;
  shared_ptr<Material> matPtr;
};

bool Sphere::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const
{
  Vec3 oc = r.origin() - center;
  auto a = r.direction().lengthSquared();
  auto halfB = dot(oc, r.direction());
  auto c = oc.lengthSquared() - radius * radius;

  auto discrim = halfB * halfB - a * c;
  if (discrim < 0)
    return false;
  auto sqrtd = sqrt(discrim);

  auto root = (-halfB - sqrtd) / a;
  if (root < tMin || root > tMax)
  {
    root = (-halfB + sqrtd) / a;
    if (root < tMin || root > tMax)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  Vec3 outwardNormal = (rec.p - center) / radius;
  rec.setFaceNormal(r, outwardNormal);
  rec.matPtr = matPtr;

  return true;
}

#endif /* Sphere_h */
