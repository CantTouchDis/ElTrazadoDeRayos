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

#pragma once
#ifndef _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SHAPE_H_
#define _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SHAPE_H_

// GlMath include.
#include <glm/glm.hpp>

#include <vector>

class Ray;

// This abstract class is used to define essencial parts to render a "Shape"
class Shape {
 public:
  virtual ~Shape() { };
  // Multiplies the matrix with the current Transformation.
  virtual void transform(const glm::mat4& matrix);
  // Intersects the Ray with this Shape and returns the values for t
  // rPos + rDir * t that intersect the surface of this Shape.
  virtual std::vector<double> intersect(const Ray& ray) const = 0;
  // Returnes the appearence of the surface Point p
  // if p isn't on the surface everything can happen.
  // TODO(allofus): what should be returned here?
  virtual void getAppearenceAt(const glm::vec4& p) const = 0;
 protected:
  // the transformation of this Shape
  glm::mat4 _transformation;
};


#endif  // _ELTRAZADODERAYOS_SRC_RAYTRACERLIB_SHAPE_H_