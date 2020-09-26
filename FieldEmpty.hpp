//
//  FieldEmpty.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-13.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include "Field.hpp"

// an empty field that exists only for testing purposes
class FieldEmpty : public Field {
    glm::vec3 origin;
    float radius;
public:
    // constructor
    FieldEmpty(glm::vec3 origin, float radius);
    
    // ray differential equation function
    glm::vec3 deltaRay(Ray ray);
};
