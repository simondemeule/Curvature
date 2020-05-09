//
//  MeshPrimitive.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "ShadableObject.hpp"
#include "ShadableObjectIntersection.hpp"
#include "DistanceMeasure.hpp"

class MeshInstance; // forward declare

class MeshPrimitive : public ShadableObject {
private:
    void updateBoundingBox();
public:
    MeshInstance* meshInstance;
    int indexLocation;
    
    MeshPrimitive(MeshInstance* meshInstance, int indexLocation);
    
    // intersection function
    ShadableObjectIntersection intersection(Ray ray);
    // distance function
    DistanceMeasure distance(glm::vec3 point);
};
