#include <iostream>
#include <fstream>
#include <Magick++.h>

#include "Camera.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Materials.hpp"

#define StrEq(s1, s2) (strcmp(s1, s2) == 0)
#define PrefixEq(s1, s2, n) (strncmp(s1, s2, n) == 0)

Color rayColor(const Ray &r, const Hittable &world, int depth) {
  HitRecord rec;
  
  // If we've exceeded the ray bounce limit, no more light is gathered
  if (depth <= 0)
    return Color(0, 0, 0);
  
  if (world.hit(r, 0.001, Infinity, rec)) {
    Ray scattered;
    Color attenuation;
    if (rec.matPtr->scatter(r, rec, attenuation, scattered))
      return attenuation * rayColor(scattered, world, depth - 1);
    return Color(0, 0, 0);
  }
  
  Vec3 unitDir = unitVector(r.direction());
  auto t = 0.5 * (unitDir.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

// TODO: Push things to a namespace for cleaner code.
int main(int argc, char **argv) {
  Magick::InitializeMagick(*argv);

  // Image
  const auto AspectRatio = 16.0 / 9.0;
  const int ImageWidth = 640;
  const int ImageHeight = static_cast<int>(ImageWidth / AspectRatio);
  const int SamplesPerPixel = 100;
  const int MaxRayDepth = 12;

  // World
  HittableList world;

  auto materialGround = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto materialCenter = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto materialLeft = std::make_shared<Dielectric>(1.45);
  auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

  world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, materialGround));
  world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, materialCenter));
  world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), 0.5, materialLeft));
  world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), -0.45, materialLeft));
  world.add(std::make_shared<Sphere>(Point3(1, 0, -1), 0.5, materialRight));

  // Camera
  Point3 lookFrom(3, 3, 2);
  Point3 lookAt(1, 0, -1);
  Vec3 vup(0, 1, 0);
  auto distToFocus = (lookFrom - lookAt).length();
  auto aperture = 2.0;

  Camera cam(lookFrom, lookAt, vup, 35, AspectRatio, aperture, distToFocus);
  
  // Arguments
  std::string outFileName = "./out.ppm";
  
  for (int i = 1; i < argc; i++) {
    if (StrEq(argv[i], "-v") || StrEq(argv[i], "--version")) {
      std::cout << "Lark " << ConfigVersion
                << " (" << ConfigGitBranch << ", "
                << ConfigGitRev << ")" << std::endl;
      std::cout << "Built " << ConfigBuildDate << std::endl; 

      return 0;
    } else if (StrEq(argv[i], "-o") && StrEq(argv[i], "--output") && PrefixEq(argv[i], "--output", 8)) {
      if (PrefixEq(argv[i], "--output", 8) && strlen(argv[i]) >= 8) {
        // --output=<value>
        std::string value = argv[i];
        outFileName = value.substr(0, 9); // <value>
        
        if (outFileName.empty()) {
          std::cerr << "error: expected value for output file" << std::endl
          << "you may optionally omit this option to output to './out.ppm'" << std::endl;
          return 1;
        }
        continue;
      }
      
      // --output <value>
      // -o <value>
      if (i + 1 >= argc) {
        std::cerr << "error: expected value for output file" << std::endl
        << "you may optionally omit this option to output to './out.ppm'" << std::endl;
        return 1;
      }
      
      outFileName = argv[++i];
    }
  }
  
  std::cout << "out file: " << outFileName << std::endl;
  std::ofstream outFile(outFileName, std::ofstream::out | std::ofstream::trunc);

  outFile << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

  // Render
  for (int j = ImageHeight - 1; j >= 0; j--) {
    std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < ImageWidth; i++) {
      Color pixelColor(0, 0, 0);
      
      for (int s = 0; s < SamplesPerPixel; ++s) {
        auto u = (i + randomDouble()) / (ImageWidth-1);
        auto v = (j + randomDouble()) / (ImageHeight-1);
        Ray r = cam.getRay(u, v);
        pixelColor += rayColor(r, world, MaxRayDepth);
      }
      
      writeColor(outFile, pixelColor, SamplesPerPixel);
    }
  }

  std::cout << "\nConverting... ";
  outFile.close();

  Magick::Image image;
  try {
    image.read(outFileName);

    image.write(outFileName.substr(0, outFileName.find_last_of('.')) + ".png");
  } catch (Magick::Exception &error_) { 
    std::cout << "\nCaught exception: " << error_.what() << std::endl; 
    return 1; 
  }

  std::cout << "Done.\n";

  return 0;
}
