//
//  Portal.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-04-12.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include "BoundingBox.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

// a portal
class Portal {
public:
    BoundingBox boundingBox;
    
    virtual Intersection intersection(Ray ray) = 0;
    virtual Ray traverse(Ray incoming) = 0;
};
