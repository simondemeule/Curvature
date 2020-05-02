//
//  BoundingBox.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "BoundingBox.hpp"

BoundingBox::BoundingBox() :
    pointPositive(glm::vec3(0)),
    pointNegative(glm::vec3(0))
{}

BoundingBox::BoundingBox(glm::vec3 pointPositiveNew, glm::vec3 pointNegativeNew) :
    pointPositive(pointPositiveNew),
    pointNegative(pointNegativeNew)
{}

BoundingBox::BoundingBox(BoundingBox &firstBox, BoundingBox &secondBox) :
    pointPositive(glm::max(firstBox.pointPositive, secondBox.pointPositive)),
    pointNegative(glm::min(firstBox.pointNegative, secondBox.pointNegative))
{}

void BoundingBox::setPointPositive(glm::vec3 pointPositiveNew) {
    pointPositive = pointPositiveNew;
}

void BoundingBox::setPointNegative(glm::vec3 pointNegativeNew) {
    pointNegative = pointNegativeNew;
}

bool BoundingBox::intersectionTest(Ray ray) {
    // code based on scratchapixel's tutorial. see https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    
    glm::vec3 inverse = glm::vec3(1.0) / ray.direction;
    
    glm::bvec3 sign;
    
    sign.x = inverse.x > 0;
    sign.y = inverse.y > 0;
    sign.z = inverse.z > 0;
    
    glm::vec3 minLength;
    glm::vec3 maxLength;
    
    minLength.x = ((sign.x ? pointNegative : pointPositive).x - ray.origin.x) * inverse.x;
    maxLength.x = ((!sign.x ? pointNegative : pointPositive).x - ray.origin.x) * inverse.x;
    minLength.y = ((sign.y ? pointNegative : pointPositive).y - ray.origin.y) * inverse.y;
    maxLength.y = ((!sign.y ? pointNegative : pointPositive).y - ray.origin.y) * inverse.y;
    
    if ((minLength.x > maxLength.y) || (minLength.y > maxLength.x))
        return false;
    if (minLength.y > minLength.x)
        minLength.x = minLength.y;
    if (maxLength.y < maxLength.x)
        maxLength.x = maxLength.y;
    
    minLength.z = ((sign.z ? pointNegative : pointPositive).z - ray.origin.z) * inverse.z;
    maxLength.z = ((!sign.z ? pointNegative : pointPositive).z - ray.origin.z) * inverse.z;
    
    if ((minLength.x > maxLength.z) || (minLength.z > maxLength.x))
        return false;
//    if (minLength.z > minLength.x)
//        minLength.x = minLength.z;
//    if (maxLength.z < maxLength.x)
//        maxLength.x = maxLength.z;
    
    return true;
}

bool BoundingBox::overlapTest(BoundingBox boundingBox) {
    return  boundingBox.pointNegative.x <= pointPositive.x && boundingBox.pointPositive.x >= pointNegative.x &&
            boundingBox.pointNegative.y <= pointPositive.y && boundingBox.pointPositive.y >= pointNegative.y &&
            boundingBox.pointNegative.z <= pointPositive.z && boundingBox.pointPositive.z >= pointNegative.z;
}

bool BoundingBox::containmentTest(Ray ray) {
    return (ray.origin.x <= pointPositive.x) && (ray.origin.x >= pointNegative.x) &&
           (ray.origin.y <= pointPositive.y) && (ray.origin.y >= pointNegative.y) &&
           (ray.origin.z <= pointPositive.z) && (ray.origin.z >= pointNegative.z);
}

float BoundingBox::surfaceArea() {
    return 2 * (pointPositive.x - pointNegative.x) * (pointPositive.y - pointNegative.y) + 2 * (pointPositive.x - pointNegative.x) * (pointPositive.z - pointNegative.z) + 2 * (pointPositive.y - pointNegative.y) * (pointPositive.z - pointNegative.z);
    ;
}