//
//  DistanceMeasure.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-09.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

struct DistanceMeasure {
    float distance;
    glm::vec3 origin;
    
    int objectDistanceDepth = 0;
    int boxDistanceDepth = 0;
    int debugMarker = 0;
};
