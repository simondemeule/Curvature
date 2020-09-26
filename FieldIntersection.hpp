//
//  FieldIntersection.hpp
//  Curvature
//
//  Created by Simon Demeule on 2020-05-12.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Intersection.hpp"

class Field; // forward declare

// shadable object intersection, contains pointer to shadable object and more detailed data about the intersection
struct FieldIntersection : public Intersection {
public:
    glm::vec3 origin;
    glm::vec3 incident;
    
    int objectIntersectionDepth = 0;
    int boxIntersectionDepth = 0;
    int debugMarker = 0;
    
    Field *object;
};
