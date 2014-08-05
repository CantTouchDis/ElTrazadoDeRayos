/*
The MIT License (MIT)

Copyright (c) 2014 CantTouchDis <bauschp@informatik.uni-freiburg.de>
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

#include "./ImageNormalizer.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include "../Color.h"
#include "../Image.h"

// _____________________________________________________________________________
void ImageNormalizer::doPostProcess(Image* imagePtr,
                                   const size_t& startPixel,
                                   const size_t& endPixel) const {
  // Loop over all pixels and correct them.
  float globalMax = -std::numeric_limits<float>::max();
  #pragma omp parallel
  {
    float tMax = -std::numeric_limits<float>::max();
    #pragma omp for schedule(dynamic, 100)
    for (int i = startPixel; i < static_cast<int>(endPixel); ++i) {
      // Get the pixel coordinates from i.
      int x = i % imagePtr->getWidth();
      int y = i / imagePtr->getWidth();
      // Get the color.
      const Color& color = imagePtr->getPixel(x, y);
      tMax = std::max(std::max(tMax, color.r()),
                      std::max(color.g(), color.b()));
    }
    #pragma omp critical
    {
      printf("max %f\n", tMax);
      globalMax = std::max(globalMax, tMax);
    }
  }

  // Normalize them.
  float invMax = 1.0f / globalMax;
  #pragma omp parallel for schedule(dynamic, 100)
  for (int i = startPixel; i < static_cast<int>(endPixel); ++i) {
    // Get the pixel coordinates from i.
    int x = i % imagePtr->getWidth();
    int y = i / imagePtr->getWidth();
    // Get the color.
    const Color& color = imagePtr->getPixel(x, y);
    // Set the corrected color.
    Color correctedColor(color.r() * invMax,
                         color.g() * invMax,
                         color.b() * invMax);
    imagePtr->setPixel(x, y, correctedColor);
  }
}

