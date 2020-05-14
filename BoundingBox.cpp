//
//  BoundingBox.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-29.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "BoundingBox.hpp"

#include <algorithm>

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

BoundedObjectIntersection BoundingBox::intersection(Ray ray) {
    // code based on scratchapixel's tutorial. see https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    BoundedObjectIntersection intersection;
    
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
    
    if ((minLength.x > maxLength.y) || (minLength.y > maxLength.x)) {
        intersection.exists = false;
        return intersection;
    }
    if (minLength.y > minLength.x) {
        minLength.x = minLength.y;
    }
    if (maxLength.y < maxLength.x) {
        maxLength.x = maxLength.y;
    }
    
    minLength.z = ((sign.z ? pointNegative : pointPositive).z - ray.origin.z) * inverse.z;
    maxLength.z = ((!sign.z ? pointNegative : pointPositive).z - ray.origin.z) * inverse.z;
    
    if ((minLength.x > maxLength.z) || (minLength.z > maxLength.x)) {
        intersection.exists = false;
        return intersection;
    }
    if (minLength.z > minLength.x) {
        minLength.x = minLength.z;
    }
    if (maxLength.z < maxLength.x) {
        maxLength.x = maxLength.z;
    }
    
    intersection.exists = true;
    intersection.distance = minLength.x;
    return intersection;
    
}

// taken from https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
DistanceMeasure BoundingBox::distance(glm::vec3 point) {
    glm::vec3 pointCenter = 0.5f * (pointPositive + pointNegative);
    glm::vec3 boundFromCenter = pointPositive - pointCenter;
    glm::vec3 pointFromCenter = point - pointCenter;
    glm::vec3 pointFromCornerMirrored = abs(pointFromCenter) - boundFromCenter;
    
    DistanceMeasure distanceMeasure;
    distanceMeasure.origin = point;
    distanceMeasure.boxDistanceDepth = 1;
    distanceMeasure.distance = glm::length(glm::max(pointFromCornerMirrored, glm::vec3(0.0))) + std::min<float>(std::max<float>(pointFromCornerMirrored.x, std::max<float>(pointFromCornerMirrored.y, pointFromCornerMirrored.z)), 0.0);
    
    return distanceMeasure;
}

// original code
// the furthest point is always a box corner. relative to the center, the sign of the corter is the opposite of the point
DistanceMeasure BoundingBox::distanceEnd(glm::vec3 point) {
    glm::vec3 pointCenter = 0.5f * (pointPositive + pointNegative);
    glm::vec3 boundFromCenter = pointPositive - pointCenter;
    glm::vec3 pointFromCenter = point - pointCenter;
    
    glm::bvec3 sign;
    
    sign.x = pointFromCenter.x > 0;
    sign.y = pointFromCenter.y > 0;
    sign.z = pointFromCenter.z > 0;
    
    glm::vec3 pointFurthestFromCenter;
    
    pointFurthestFromCenter.x = sign.x ? - boundFromCenter.x : boundFromCenter.x;
    pointFurthestFromCenter.y = sign.y ? - boundFromCenter.y : boundFromCenter.y;
    pointFurthestFromCenter.z = sign.z ? - boundFromCenter.z : boundFromCenter.z;
    
    DistanceMeasure distanceMeasure;
    distanceMeasure.origin = point;
    distanceMeasure.boxDistanceDepth = 1;
    distanceMeasure.distance = glm::length(pointFromCenter - pointFurthestFromCenter);
    
    return distanceMeasure;
}

float BoundingBox::surfaceArea() {
    return 2 * (pointPositive.x - pointNegative.x) * (pointPositive.y - pointNegative.y) + 2 * (pointPositive.x - pointNegative.x) * (pointPositive.z - pointNegative.z) + 2 * (pointPositive.y - pointNegative.y) * (pointPositive.z - pointNegative.z);
    ;
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
    
    return true;
}

bool BoundingBox::containmentTest(BoundingBox boundingBox) {
    return  boundingBox.pointNegative.x <= pointPositive.x && boundingBox.pointPositive.x >= pointNegative.x &&
            boundingBox.pointNegative.y <= pointPositive.y && boundingBox.pointPositive.y >= pointNegative.y &&
            boundingBox.pointNegative.z <= pointPositive.z && boundingBox.pointPositive.z >= pointNegative.z;
}

bool BoundingBox::containmentTest(glm::vec3 point) {
    return (point.x <= pointPositive.x) && (point.x >= pointNegative.x) &&
           (point.y <= pointPositive.y) && (point.y >= pointNegative.y) &&
           (point.z <= pointPositive.z) && (point.z >= pointNegative.z);
}

bool BoundingBox::containmentTest(Ray ray) {
    return containmentTest(ray.origin);
}

bool BoundingBox::overlapTest(BoundingBox boundingBox) {
    return pointPositive.x >= boundingBox.pointNegative.x && pointNegative.x <= boundingBox.pointPositive.x &&
           pointPositive.y >= boundingBox.pointNegative.y && pointNegative.y <= boundingBox.pointPositive.y &&
           pointPositive.z >= boundingBox.pointNegative.z && pointNegative.z <= boundingBox.pointPositive.z;
}

bool BoundingBox::overlapNonZeroTest(BoundingBox boundingBox) {
    return pointPositive.x > boundingBox.pointNegative.x && pointNegative.x < boundingBox.pointPositive.x &&
           pointPositive.y > boundingBox.pointNegative.y && pointNegative.y < boundingBox.pointPositive.y &&
           pointPositive.z > boundingBox.pointNegative.z && pointNegative.z < boundingBox.pointPositive.z;
}
