//
//  Vec3.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/09/20.
//

#ifndef Vec3_h
#define Vec3_h

#include <cmath>
#include <iostream>
#include "Random.hpp"

using std::sqrt;

class Vec3 {
public:
  double e[3];
  
  Vec3() : e{0, 0, 0} {}
  Vec3(double x, double y, double z) : e{x, y, z} {}
  
  inline static Vec3 Random() {
    return Vec3(randomDouble(), randomDouble(), randomDouble());
  }
  
  inline static Vec3 Random(double min, double max) {
    return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
  }
  
  double x() { return e[0]; }
  double y() { return e[1]; }
  double z() { return e[2]; }
  
  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  Vec3& operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vec3& operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3& operator/=(const double t) {
    return *this *= 1/t;
  }

  double length() const {
    return sqrt(lengthSquared());
  }

  double lengthSquared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  bool nearZero() const {
    const auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
  }
};

using Point3 = Vec3;
using Color = Vec3;

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
  return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
  return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
  return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unitVector(Vec3 v) {
  return v / v.length();
}

inline Vec3 randomInUnitSphere() {
  while (true) {
    auto p = Vec3::Random(-1, 1);
    if (p.lengthSquared() >= 1) continue;
    
    return p;
  }
}

Vec3 randomUnitVector() {
  return unitVector(randomInUnitSphere());
}

Vec3 randomInHemisphere(const Vec3 &normal) {
  Vec3 inUnitSphere = randomInUnitSphere();
  if (dot(inUnitSphere, normal) > 0.0) // in the same hemisphere as the normal
    return inUnitSphere;
  else
    return -inUnitSphere;
}

Vec3 randomInUnitDisk() {
  while (true) {
    auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
    if (p.lengthSquared() >= 1) continue;
    
    return p;
  }
}

Vec3 reflect(const Vec3 &v, const Vec3 &n) {
  return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3 &uv, const Vec3 &n, double etaiOverEtat) {
  auto cosTheta = fmin(dot(-uv, n), 1.0);
  Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
  Vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * n;
  return rOutPerp + rOutParallel;
}

#endif /* Vec3_h */
