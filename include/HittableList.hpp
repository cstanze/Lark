//
//  HittableList.h
//  lark
//
//  Created by Jules Amalie on 2022/10/04.
//

#ifndef HittableList_h
#define HittableList_h

#include "Hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }
  
  void clear() { objects.clear(); }
  void add(shared_ptr<Hittable> object) { objects.push_back(object); }
  
  virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const override;

public:
  std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const {
  HitRecord tempRec;
  bool hitAnything = false;
  auto closestSoFar = tMax;
  
  for (const auto &object : objects) {
    if (object->hit(r, tMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      rec = tempRec;
    }
  }
  
  return hitAnything;
}

#endif /* HittableList_h */
