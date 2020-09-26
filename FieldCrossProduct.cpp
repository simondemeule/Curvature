//
//  FieldCrossProduct.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-12.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "FieldCrossProduct.hpp"

#include <algorithm>

FieldCrossProduct::FieldCrossProduct(glm::vec3 originNew, glm::vec3 directionNew, float radiusNew, float strengthNew) :
    origin(originNew),
    direction(directionNew),
    radius(radiusNew),
    strength(strengthNew)
{
    boundingBox.setPointPositive(glm::vec3(radiusNew) + originNew);
    boundingBox.setPointNegative(- glm::vec3(radiusNew) + originNew);
    primitiveCount = 1;
}

glm::vec3 FieldCrossProduct::deltaRay(Ray ray) {
    float intensity = strength * (0.5 + 0.5 * std::cos(M_PI * std::min(glm::length(ray.origin - origin) / radius, 1.0f)));
    return intensity * glm::cross(ray.direction, direction);
}
