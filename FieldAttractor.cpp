//
//  FieldAttractor.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-13.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "FieldAttractor.hpp"

#include <algorithm>

FieldAttractor::FieldAttractor(glm::vec3 originNew, float radiusNew, float strengthNew) :
origin(originNew),
radius(radiusNew),
strength(strengthNew)
{
    boundingBox.setPointPositive(glm::vec3(radiusNew) + originNew);
    boundingBox.setPointNegative(- glm::vec3(radiusNew) + originNew);
    primitiveCount = 1;
}

glm::vec3 FieldAttractor::deltaRay(Ray ray, float step) {
    float intensity = strength * (0.5 + 0.5 * std::cos(M_PI * std::min(glm::length(ray.origin - origin) / radius, 1.0f)));
    return intensity * step * glm::normalize(glm::vec3(origin - ray.origin));
}
