//
//  Camera.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/10/12.
//

#ifndef Camera_h
#define Camera_h

#include "Utils.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

class Camera {
  Point3 Origin;
  Point3 LowerLeftCorner;
  Vec3 Horiz;
  Vec3 Vert;
  Vec3 u, v, w;
  double lensRadius;
  
public:
  Camera(
    Point3 lookFrom,
    Point3 lookAt,
    Vec3 vup,
    double vFov,
    double AspectRatio,
    double aperture,
    double focusDist
  ) {
    auto theta = degreesToRadians(vFov);
    auto h = tan(theta / 2);
    auto ViewportHeight = 2.0 * h;
    auto ViewportWidth = AspectRatio * ViewportHeight;

    w = unitVector(lookFrom - lookAt);
    u = unitVector(cross(vup, w));
    v = cross(w, u);
    
    Origin = lookFrom;
    Horiz = focusDist * ViewportWidth * u;
    Vert = focusDist * ViewportHeight * v;
    LowerLeftCorner = Origin - Horiz / 2 - Vert / 2 - focusDist * w;

    lensRadius = aperture / 2;
  }
  
  Ray getRay(double s, double t) {
    Vec3 rd = lensRadius * randomInUnitDisk();
    Vec3 offset = u * rd.x() + v * rd.y();

    return Ray(Origin + offset, LowerLeftCorner + s * Horiz + t * Vert - Origin - offset);
  }
};

#endif /* Camera_h */
