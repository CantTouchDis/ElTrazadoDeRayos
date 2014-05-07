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

// This header probvides all nessecery constants for the Tracer Lib.
#pragma once
#ifndef RAYTRACERLIB_CONSTANTS_H_
#define RAYTRACERLIB_CONSTANTS_H_

// TODO(bauschp): Create a Variable for floating point precision.
//                Maybe use #define name double/float

typedef double REAL;

namespace constants {
  // This defines how precise a floating point variable has to be to be
  // approximatly the value.
  // e.g. check for zero:
  // bool isZero(float f) { return (f < EPSILON) && (f > -EPSILON); }
  const double EPSILON = 1e-5;
}  // namespace constants
#endif  // RAYTRACERLIB_CONSTANTS_H_