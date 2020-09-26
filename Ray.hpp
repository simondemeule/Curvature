//
//  Ray.hpp
//  Curvature
//
//  Created by Simon Demeule on 2019-03-20.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

// ray data structure
struct Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    bool isCameraRay;
};
