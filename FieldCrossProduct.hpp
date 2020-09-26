//
//  FieldCrossProduct.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-12.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include "Field.hpp"

// a field that acts much like a magnetic monopole deflecting the path of charged particles
class FieldCrossProduct : public Field {
    glm::vec3 origin;
    glm::vec3 direction;
    float radius;
    float strength;
public:
    // constructor
    FieldCrossProduct(glm::vec3 origin, glm::vec3 direction, float radius, float strength);
    
    // ray differential equation function
    glm::vec3 deltaRay(Ray ray);
};
