//
//  Sphere.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadableObject.hpp"
#include "ShadableAttributes.hpp"
#include "ShadableObjectIntersection.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

// sphere class
class Sphere : public ShadableObject {
private:
    glm::vec3 origin;
    float radius;

public:
    Sphere(glm::vec3 originNew, float radiusNew, ShadableAttributes* shadableAttributesNew);
    
    ShadableObjectIntersection intersection(Ray ray);
};
