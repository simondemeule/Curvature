//
//  Sphere.hpp
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
#include "BoundingBox.hpp"

// sphere class
class Sphere : public ShadingObject {
private:
    glm::vec3 origin;
    float radius;

public:
    Sphere(glm::vec3 originNew, float radiusNew, ShadingAttributes shadingAttributesNew);
    
    Intersection intersection(Ray ray);
};
