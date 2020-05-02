//
//  Intersection.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-21.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

class ShadingObject; // forward declare

// intersection data structure
struct Intersection {
public:
    bool exists;
    glm::vec3 origin;
    glm::vec3 normal;
    glm::vec3 incident;
    float distance;
    ShadingObject *shadingObject;
};
