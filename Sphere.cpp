//
//  Sphere.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 originNew, float radiusNew, ShadableAttributes shadingAttributesNew) :
    ShadableObject(shadingAttributesNew),
    origin(originNew),
    radius(radiusNew)
{
    boundingBox.setPointPositive(glm::vec3(radiusNew, radiusNew, radiusNew) + originNew);
    boundingBox.setPointNegative(glm::vec3(- radiusNew, - radiusNew, - radiusNew) + originNew);
    primitiveCount = 1;
}

ShadableObjectIntersection Sphere::intersection(Ray ray) {
    ShadableObjectIntersection intersection;
    // code based on class notes
    glm::vec3 delta = ray.origin - origin;
    float b = 2.0 * glm::dot(ray.direction, delta);
    float c = glm::dot(delta, delta) - radius * radius;
    float n = b * b - 4.0 * c;
    if(n < 0) {
        // root doesn't exist
        // exit calculation
        intersection.exists = false;
        return intersection;
    }
    float r = std::sqrt(n);
    float t = (- b - r) / 2.0;
    // the intersection with smallest distance is
    // tmin = (- b - r) / 2.0
    //
    // the intersection with largest distance is
    // tmax (- b + r) / 2.0
    //
    // in all cases, tmax geometrically corresponds to a backface intersection.
    // this means we are only interested in tmin.
    //
    // if the ray is a camera ray, positive bias prevents intersection if the camera is at the surface of the sphere.
    // if the ray is not a camera ray, negative bias ensures that objects which spawn a shadow ray on their surface will generate an intersection.
    if(t < (ray.isCameraRay ? 1e-2 : -1e-2)) {
        // the intersection is behind the ray's origin
        // exit calculation
        intersection.exists = false;
        return intersection;
    }
    intersection.origin = ray.origin + ray.direction * t;
    intersection.normal = (intersection.origin - origin) * (float)(1.0 / radius);
    intersection.exists = true;
    intersection.incident = ray.direction;
    intersection.distance = t;
    intersection.shadableObject = this;
    return intersection;
}
