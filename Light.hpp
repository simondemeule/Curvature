//
//  Light.hpp
//  project
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadingAttributes.hpp"

// light data structure
class Light {
public:
    ShadingAttributes shadingAttributes;
    glm::vec3 origin;
    
    Light(glm::vec3 originNew, ShadingAttributes shadingAttributesNew);
};
