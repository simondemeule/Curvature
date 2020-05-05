//
//  Light.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadableAttributes.hpp"

// light data structure
class Light {
public:
    ShadableAttributes shadableAttributes;
    glm::vec3 origin;
    
    Light(glm::vec3 originNew, ShadableAttributes shadableAttributesNew);
};
