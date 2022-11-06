 //
//  Random.hpp
//  Lark
//
//  Created by Jules Amalie on 2022/11/5.
//

#ifndef Random_h
#define Random_h

#include <random> 

inline double randomDouble() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double min, double max) {
  return min + (max - min) * randomDouble();
}

#endif

