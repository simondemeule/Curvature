//
//  Shadable.hpp
//  Curvature
//
//  Created by Simon Demeule on 2019-03-20.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadableAttributes.hpp"
#include "ShadableObjectIntersection.hpp"
#include "DistanceMeasure.hpp"
#include "Ray.hpp"
#include "BoundedObject.hpp"

// shadable object class
class ShadableObject : public BoundedObject {
public:
    // shading attributes (ambient, diffuse, specular, etc)
    ShadableAttributes* shadableAttributes;
    
    // constructor
    ShadableObject(ShadableAttributes* shadableAttributesNew) : shadableAttributes(shadableAttributesNew) {}
    
    // intersection function
    virtual ShadableObjectIntersection intersection(Ray ray) = 0;
    // distance function
    virtual DistanceMeasure distance(glm::vec3 point) = 0;
    
    // primitive count for bounded hierarchy construction optimisation
    int primitiveCount;
};
