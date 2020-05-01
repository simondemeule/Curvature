//
//  Renderable.hpp
//  project
//
//  Created by Simon Demeule on 2019-03-20.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include "ShadingAttributes.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

// shadable object class
class ShadingObject {
public:
    // shading attributes (ambient, diffuse, specular, etc)
    ShadingAttributes shadingAttributes;
    
    // constructor
    ShadingObject(ShadingAttributes shadingAttributesNew) : shadingAttributes(shadingAttributesNew) {}
    
    // intersection function for object
    virtual Intersection intersection(Ray ray) = 0;
    
    // bounding box
    BoundingBox boundingBox;
};
