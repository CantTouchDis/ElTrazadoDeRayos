/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "./PerspectiveCamera.h"
#include <cmath>
#include <ctime>
#include "./Constants.h"
#include "./Ray.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Material.h"

// _____________________________________________________________________________
PerspectiveCamera::PerspectiveCamera(const int width,
      const int height,
      const REAL& fovAngleInRad) : Camera(width, height) {
  // TODO(bauschp): Insert right formula.
  _focalLength = width / fovAngleInRad;
}

// _____________________________________________________________________________
PerspectiveCamera::~PerspectiveCamera() { }

// _____________________________________________________________________________
void PerspectiveCamera::render(const Scene& scene) {
  // Get the size of the image.
  REAL startX = _image.getWidth() / 2.0;
  REAL startY = _image.getHeight() / 2.0;

  glm::vec4 position(0, 0, 0, 1);
  position = _transformation * position;
  // Send rays.
  size_t overallRayCreation = 0;
  size_t overallSceneTrace = 0;
  size_t overallGetColor = 0;
  for (int x = 0; x < _image.getWidth(); ++x) {
    for (int y = 0; y < _image.getHeight(); ++y) {
      clock_t start = clock();
      // Create new direction.
      glm::vec4 direction(-startX + x, startY - y, -_focalLength, 0);
      direction = glm::normalize(direction);
      direction = _transformation * direction;

      Ray r(position, direction);
      clock_t end = clock();
      overallRayCreation += end - start;
      // if (y == 0)
      // printf("SENDING RAY:(%.2f,%.2f,%.2f|%.2f,%.2f,%.2f)\n", position.x,
      //       position.y, position.z, direction.x, direction.y, direction.z);
      start = end;
      IntersectionInfo info = scene.traceRay(r);
      end = clock();
      overallSceneTrace += end - start;
      start = end;
      if (info.materialPtr) {
        // HIT
        Color tmpColor = info.materialPtr->getColor(info.hitPoint,
                                                    info.normal,
                                                    r,
                                                    scene);
        _image.setPixel(x, y, tmpColor);
      } else {
        _image.setPixel(x, y, Color(1, 1, 1, 1));
      }
      end = clock();
      overallGetColor += end - start;
    }
  }
  printf("\tRay creation took: %.2f sec.\n",
      static_cast<float>(overallRayCreation) / CLOCKS_PER_SEC);
  printf("\tTrace took: %.2f sec.\n",
      static_cast<float>(overallSceneTrace) / CLOCKS_PER_SEC);
  printf("\tGetColor took: %.2f sec.\n",
      static_cast<float>(overallGetColor) / CLOCKS_PER_SEC);
}

