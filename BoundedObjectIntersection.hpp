//
//  BoundedObjectIntersection.hpp
//  Curvature
//
//  Created by Simon Demeule on 2020-05-04.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include "Intersection.hpp"

class BoundedObject; // forward declare

// shadable object intersection, contains pointer to bounded object
struct BoundedObjectIntersection : public Intersection {
public:
    BoundedObject *boundedObject;
};
