//
//  Plane.hpp
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

// TODO: currently the bounding box is infinite. This is stupid and might break things.

// plane class
class Plane : public ShadableObject {
private:
    glm::vec3 origin;
    glm::vec3 normal;
public:
    Plane(glm::vec3 originNew, glm::vec3 normalNew, ShadableAttributes* shadableAttributesNew);
    
    ShadableObjectIntersection intersection(Ray ray);
};
