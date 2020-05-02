//
//  Plane.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadingObject.hpp"
#include "ShadingAttributes.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

// TODO: currently the bounding box is infinite. This is stupid and might break things.

// plane class
class Plane : public ShadingObject {
private:
    glm::vec3 origin;
    glm::vec3 normal;
public:
    Plane(glm::vec3 originNew, glm::vec3 normalNew, ShadingAttributes shadingAttributesNew);
    
    Intersection intersection(Ray ray);
};
