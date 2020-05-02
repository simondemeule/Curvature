//
//  Field.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "BoundingBox.hpp"
#include "Ray.hpp"

// the parent class of all field types. deltaRay defines light-bending behaviour and is implemented by a child.
class Field {
public:
    BoundingBox boundingBox;
    
    virtual glm::vec3 deltaRay(Ray ray, float step) = 0;
};
