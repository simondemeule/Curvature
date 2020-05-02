//
//  Plane.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane(glm::vec3 originNew, glm::vec3 normalNew, ShadingAttributes shadingAttributesNew) :
    origin(originNew),
    normal(glm::normalize(normalNew)),
    ShadingObject(shadingAttributesNew)
{
    float infinity = std::numeric_limits<float>::infinity();
    boundingBox.setPointPositive(glm::vec3(infinity));
    boundingBox.setPointNegative(glm::vec3(-infinity));
    primitiveCount = 1;
}

Intersection Plane::intersection(Ray ray) {
    // code based on scratchapixel's tutorial. see https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    Intersection intersection;
    float denom = glm::dot(normal, ray.direction);
    if(denom > -1e-6) {
        // intersection is a backface or ray is parallel to plane
        // exit calculation
        intersection.exists = false;
        return intersection;
    }
    float t = glm::dot(origin - ray.origin, normal) / denom;
    if(t < 1e-3) {
        // intersection is behind ray
        // exit calculation
        intersection.exists = false;
        return intersection;
    }
    // intersection is valid
    intersection.exists = true;
    intersection.origin = ray.origin + ray.direction * t;
    intersection.normal = normal;
    intersection.incident = ray.direction;
    intersection.distance = t;
    intersection.shadingObject = this;
    return intersection;
};
