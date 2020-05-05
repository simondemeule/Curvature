//
//  MeshPrimitive.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include "ShadableObject.hpp"

class MeshInstance; // forward declare

class MeshPrimitive : public ShadableObject {
private:
    void updateBoundingBox();
public:
    MeshInstance* meshInstance;
    int indexLocation;
    
    MeshPrimitive(MeshInstance* meshInstance, int indexLocation);
    
    ShadableObjectIntersection intersection(Ray ray);
};
