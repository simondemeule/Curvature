//
//  FieldAttractor.hpp
//  Curvature
//
//  Created by Simon Demeule on 2020-05-13.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include "Field.hpp"

// a field acting roughly like an attractive electric force
class FieldAttractor : public Field {
    glm::vec3 origin;
    float radius;
    float strength;
public:
    // constructor
    FieldAttractor(glm::vec3 origin, float radius, float strength);
    
    // ray differential equation function
    glm::vec3 deltaRay(Ray ray);
};
