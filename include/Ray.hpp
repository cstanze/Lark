//
//  Ray.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/09/27.
//

#ifndef Ray_h
#define Ray_h

#include "Vec3.hpp"

class Ray {
public:
  Point3 orig;
  Vec3 dir;
  
  Ray() {}
  Ray(Point3 orig, Vec3 dir) : orig(orig), dir(dir) {}
  
  Point3 origin() const { return orig; }
  Vec3 direction() const { return dir; }
  
  Point3 at(double t) const {
    return orig + t*dir;
  }
};

#endif /* Ray_h */
