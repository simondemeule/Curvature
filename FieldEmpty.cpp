//
//  FieldEmpty.cpp
//  Curvature
//
//  Created by Simon Demeule on 2020-05-13.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "FieldEmpty.hpp"

FieldEmpty::FieldEmpty(glm::vec3 originNew, float radiusNew) :
origin(originNew),
radius(radiusNew)
{
    boundingBox.setPointPositive(glm::vec3(radiusNew) + originNew);
    boundingBox.setPointNegative(- glm::vec3(radiusNew) + originNew);
    primitiveCount = 1;
}

glm::vec3 FieldEmpty::deltaRay(Ray ray) {
    return glm::vec3(0);
}
