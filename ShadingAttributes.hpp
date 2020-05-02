//
//  ShadingAttributes.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-20.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

// shading attributes data structure
struct ShadingAttributes {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float reflectivity;
};
