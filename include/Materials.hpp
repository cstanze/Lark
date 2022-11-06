//
//  Materials.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/11/5.
//

#ifndef Materials_h
#define Materials_h

#include "Material.hpp"
#include "Hittable.hpp"

class Lambertian : public Material {
public:
  Color albedo;

  Lambertian(const Color &a) : albedo(a) {}

  virtual bool scatter(
      const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
  {
    auto scatterDirection = rec.normal + randomUnitVector();
    
    // Catch degenerate scatter direction
    if (scatterDirection.nearZero())
      scatterDirection = rec.normal;
    
    scattered = Ray(rec.p, scatterDirection);
    attenuation = albedo;

    return true;
  }
};

class Metal : public Material {
public:
  Color albedo;
  double fuzz;

  Metal(const Color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(
      const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
  {
    Vec3 reflected = reflect(unitVector(rayIn.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }
};

class Dielectric : public Material {
public:
  double ir; // Index of Refraction
  Color albedo;

  Dielectric(double ior, const Color &a = Color(1, 1, 1)) : ir(ior), albedo(a) {}

  virtual bool scatter(
    const Ray &rayIn, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
  {
    attenuation = albedo;
    double refractionRatio = rec.frontFace ? (1.0 / ir) : ir;

    Vec3 unitDirection = unitVector(rayIn.direction());
    double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    Vec3 direction;

    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
      direction = reflect(unitDirection, rec.normal);
    else
      direction = refract(unitDirection, rec.normal, refractionRatio);

    scattered = Ray(rec.p, direction);
    return true;
  }

private:
  static double reflectance(double cosine, double refIdx) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#endif
