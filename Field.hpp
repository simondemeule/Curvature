//
//  Field.hpp
//  Curvature
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "BoundedObject.hpp"
#include "FieldIntersection.hpp"
#include "Ray.hpp"

// the parent class of all field types. deltaRay defines light-bending behaviour and is implemented by a child.
class Field : public BoundedObject {
public:
    // constructor
    FieldIntersection intersection(Ray ray);
    
    // ray differential equation function
    virtual glm::vec3 deltaRay(Ray ray) = 0;
    
    // primitive count for bounded hierarchy construction optimisation
    int primitiveCount = 1;
};
