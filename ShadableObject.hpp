//
//  Shadable.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-20.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include "ShadableAttributes.hpp"
#include "ShadableObjectIntersection.hpp"
#include "Ray.hpp"
#include "BoundedObject.hpp"

// shadable object class
class ShadableObject : public BoundedObject {
public:
    // shading attributes (ambient, diffuse, specular, etc)
    ShadableAttributes shadingAttributes;
    
    // constructor
    ShadableObject(ShadableAttributes shadingAttributesNew) : shadingAttributes(shadingAttributesNew) {}
    
    // intersection function for object
    virtual ShadableObjectIntersection intersection(Ray ray) = 0;
    
    // primitive count for bounded hierarchy construction optimisation
    int primitiveCount;
};
