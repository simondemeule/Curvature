//
//  ShadableObjectIntersection.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-21.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "Intersection.hpp"

class ShadableObject; // forward declare

// shadable object intersection, contains pointer to shadable object and more detailed data about the intersection
struct ShadableObjectIntersection : public Intersection {
public:
    glm::vec3 origin;
    glm::vec3 normal;
    glm::vec3 incident;
    
    int objectIntersectionDepth = 0;
    int boxIntersectionDepth = 0;
    int debugMarker = 0;
    
    ShadableObject *shadableObject;
};
