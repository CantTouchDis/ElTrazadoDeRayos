/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis
Copyright (c) 2014 brio1009 <christoph1009@gmail.com>

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
#include "./CheckerboardMaterial.h"
#include <glm/glm.hpp>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "./Color.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./PointLight.h"
#include "./Light.h"
#include "./Ray.h"

// _____________________________________________________________________________
Color CheckerboardMaterial::getColor(const IntersectionInfo& intersectionInfo,
                                     const Ray& incomingRay,
                                     const Scene& scene) const {
  if (checkerBoard(intersectionInfo.texCoord)) {
    return _material1Ptr->getColor(intersectionInfo, incomingRay, scene);  
  } else {
    return _material2Ptr->getColor(intersectionInfo, incomingRay, scene);
  }
}

// _____________________________________________________________________________
bool CheckerboardMaterial::expandingSquares(const glm::vec2& coords) const {
  return static_cast<int>(abs(coords.x) / _uSize
                       + abs(coords.y) / _vSize + 0.5) % 2 == 0;
}

// _____________________________________________________________________________
bool CheckerboardMaterial::checkerBoard(const glm::vec2& coords) const {
  return static_cast<int>(abs(coords.x) / _uSize + 0.5) % 2 == 0
         xor static_cast<int>(abs(coords.y) / _vSize + 0.5) % 2 == 0;
}

// _____________________________________________________________________________
bool CheckerboardMaterial::dotBoard(const glm::vec2& coords) const {
  return static_cast<int>(abs(coords.x) / _uSize + 0.5) % 2 == 0
          && static_cast<int>(abs(coords.y) / _vSize + 0.5) % 2 == 0;
}
