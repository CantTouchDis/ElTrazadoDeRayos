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

#include "./MonteCarloMaterial.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include "./Color.h"
#include "./Constants.h"
#include "./IntersectionInfo.h"
#include "./Scene.h"
#include "./Light.h"
#include "./Ray.h"

using std::vector;

// _____________________________________________________________________________
MonteCarloMaterial::MonteCarloMaterial()
    : PhongMaterial() {
}

// _____________________________________________________________________________
Color MonteCarloMaterial::getColor(const IntersectionInfo& intersectionInfo,
                                   const Ray& incomingRay,
                                   const Scene& scene) const {
  // If the depth is too high we just return some color.
  // TODO(all, Mon May 26 13:49:04 CEST 2014): What color to return?
  if (incomingRay.rayInfo().depth >= constants::maxDepth) {
    return Color(0, 0, 0);
  }


  // TODO(allofus, Thu May  8 15:27:52 CEST 2014): Add to constructor.
  float ka = 0.1f;
  float kd = 0.4f;
  float ks = 0.5f;

  // cause it should be between 0 and 1.
  Color sumIntensity(0, 0, 0);

  // Number of samples in the hemisphere.
  size_t hemisphereSamples = 20;

  // The ambient light.
  Color ambientColor(0, 0, 0);

  // Weighting for each light.
  const std::vector<Light*>& lights = scene.lights();
  float lightNumWeight = 1.0f / (lights.size() + hemisphereSamples);

  // Iterate over the lights.
  for (size_t i = 0; i < lights.size(); ++i) {
    const Light* const lightPtr = lights.at(i);
    Color lightColor = lightPtr->getColor();
    // Iterate over the number of samples of this light.
    for (size_t j = 0; j < lightPtr->numberOfSamples(); ++j) {
      // Adapt the color.
      float sampleNumWeight = 1.0f / lightPtr->numberOfSamples();
      lightColor *= lightNumWeight * sampleNumWeight;
      // Calculate the new intensity.
      glm::vec4 normNormal = glm::normalize(intersectionInfo.normal);
      Ray lightRay = lightPtr->getRay(intersectionInfo.hitPoint);
      sumIntensity += diffuseTerm(lightColor,
                                  -lightRay.direction(),
                                  normNormal,
                                  kd);
      sumIntensity += specularTerm(lightColor,
                                   -lightRay.direction(),
                                   normNormal,
                                   -incomingRay.direction(),
                                   ks);
      // Add the light to the ambient term.
      ambientColor += lightColor;
    }
  }

  glm::vec3 axis = glm::cross(glm::vec3(intersectionInfo.normal),
                              glm::vec3(0, 1, 0));
  float angle = glm::angle(glm::vec3(intersectionInfo.normal),
                           glm::vec3(0, 1, 0));

  // Shoot sample rays into the hemisphere.
  for (size_t i = 0; i < hemisphereSamples; ++i) {
    // Get the sample direction.
    glm::vec4 direction = glm::vec4(glm::sphericalRand(1.0f), 0);
    direction.y = abs(direction.y);

    // Get reflected color.
    Color lightColor(0, 0, 0);
    Ray newRay;
    newRay.setDirection(glm::rotate(direction,
                                    static_cast<float>(angle),
                                    axis));
    newRay.setOrigin(intersectionInfo.hitPoint);
    newRay.rayInfo().depth = incomingRay.rayInfo().depth + 1;
    IntersectionInfo info = scene.traceRay(newRay);
    if (info.materialPtr) {
      lightColor =  info.materialPtr->getColor(info,
                                               newRay,
                                               scene);
    } else {
      lightColor = scene.backgroundColor(newRay);
    }
    // Add the color to the return intensity.
    lightColor *= lightNumWeight;
    sumIntensity += diffuseTerm(lightColor,
                                newRay.direction(),
                                intersectionInfo.normal,
                                kd);
    sumIntensity += specularTerm(lightColor,
                                 newRay.direction(),
                                 intersectionInfo.normal,
                                 -incomingRay.direction(),
                                 ks);
  }

  // Add the ambient term.
  sumIntensity += ambientTerm(ambientColor, ka);
  // Return the color.
  return color() * sumIntensity;
}
