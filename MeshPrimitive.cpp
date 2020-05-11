//
//  MeshPrimitive.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "MeshPrimitive.hpp"

#include "MeshInstance.hpp"

#include <algorithm>

MeshPrimitive::MeshPrimitive(MeshInstance* meshInstanceNew, int indexLocationNew) :
    ShadableObject(meshInstanceNew->shadableAttributes)
{
    primitiveCount = 1;
    meshInstance = meshInstanceNew;
    indexLocation = indexLocationNew;
    updateGeometryData();
    updateBoundingBox();
}

// calculates the area of a triangle given vertex positions
float area(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::length(glm::cross(b - a, c - a)) * 0.5;
}

void MeshPrimitive::updateGeometryData() {
    // get vertex indices
    int indexVertex1 = meshInstance->meshData->vertexIndices.at(indexLocation);
    int indexVertex2 = meshInstance->meshData->vertexIndices.at(indexLocation + 1);
    int indexVertex3 = meshInstance->meshData->vertexIndices.at(indexLocation + 2);
    
    // get vertex positions, applying transformation
    corner1 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertexData.at(indexVertex1), 1.0));
    corner2 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertexData.at(indexVertex2), 1.0));
    corner3 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertexData.at(indexVertex3), 1.0));
    
    // get edge vectors
    edge21 = corner2 - corner1;
    edge32 = corner3 - corner2;
    edge13 = corner1 - corner3;
    
    // get normal of triangle plane
    normalTrue = glm::normalize(glm::cross(corner2 - corner1, corner3 - corner1));
    
    // get normal indices
    int indexNormal1 = meshInstance->meshData->normalIndices.at(indexLocation);
    int indexNormal2 = meshInstance->meshData->normalIndices.at(indexLocation + 1);
    int indexNormal3 = meshInstance->meshData->normalIndices.at(indexLocation + 2);
    
    // TODO: this will crash if object has no normal data
    
    // get vertex normals, applying transformation and normalizing
    normal1 = glm::normalize(glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->normalData.at(indexNormal1), 0.0)));
    normal2 = glm::normalize(glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->normalData.at(indexNormal2), 0.0)));
    normal3 = glm::normalize(glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->normalData.at(indexNormal3), 0.0)));
    
    // compute total area
    areaTotal = area(corner1, corner2, corner3);
}

void MeshPrimitive::updateBoundingBox() {
    glm::vec3 corners[] = {corner1, corner2, corner3};
    for(int i = 0; i < 3; i++) {
        if(i == 0) {
            boundingBox.pointPositive = corners[i];
            boundingBox.pointNegative = corners[i];
        } else {
            boundingBox.pointPositive = glm::max(corners[i], boundingBox.pointPositive);
            boundingBox.pointNegative = glm::min(corners[i], boundingBox.pointNegative);
        }
    }
}

ShadableObjectIntersection MeshPrimitive::intersection(Ray ray) {    
    // calculate intersection with triangle plane
    float denom = glm::dot(normalTrue, ray.direction);
    if(denom > -1e-6) {
        // intersection with plane is a backface or ray is parallel to plane
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    float t = glm::dot(corner1 - ray.origin, normalTrue) / denom;
    if(t < 1e-3) {
        // intersection with plane is behind ray
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    
    // intersection with plane is valid
    glm::vec3 pointIntersection = ray.origin + ray.direction * t;
    
    // barycentric area computation
    float area1 = area(pointIntersection, corner2, corner3);
    float area2 = area(corner1, pointIntersection, corner3);
    float area3 = area(corner1, corner2, pointIntersection);
    
    if(std::abs((area1 + area2 + area3) / areaTotal - 1.0) > 1e-3) {
        // intersection is not in triangle
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    
    // valid intersection, compute normal
    
    // TODO: deal with "no normal data" case
    float weight1 = area1 / areaTotal;
    float weight2 = area2 / areaTotal;
    float weight3 = area3 / areaTotal;
    glm::vec3 normalInterpolated = normal1 * weight1 + normal2 * weight2 + normal3 * weight3;
    
    ShadableObjectIntersection intersection;
    intersection.exists = true;
    intersection.origin = pointIntersection;
    intersection.normal = normalInterpolated;
    intersection.distance = t;
    intersection.incident = ray.direction;
    intersection.shadableObject = this;
    return intersection;
}

// utility functions for distance computation
float sign(float number) {
    if(number > 0) {
        return 1;
    } else if(number < 0) {
        return -1;
    } else {
        return 0;
    }
}

float clamp(float number) {
    return std::min<float>(std::max<float>(number, 0), 1);
}

float dot2(glm::vec3 vector) {
    return dot(vector, vector);
    
}

// taken from https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
DistanceMeasure MeshPrimitive::distance(glm::vec3 point) {
    // get point position relative to corners
    glm::vec3 pointFrom1 = point - corner1;
    glm::vec3 pointFrom2 = point - corner2;
    glm::vec3 pointFrom3 = point - corner3;
    
    DistanceMeasure distanceMeasure;
    distanceMeasure.origin = point;
    distanceMeasure.objectDistanceDepth = 1;
    distanceMeasure.distance = sqrt(
                                        (sign(dot(cross(edge21, - normalTrue), pointFrom1)) +
                                         sign(dot(cross(edge32, - normalTrue), pointFrom2)) +
                                         sign(dot(cross(edge13, - normalTrue), pointFrom3)) < 2.0)
                                        ?
                                        std::min<float>(
                                            std::min<float>(
                                                dot2(edge21 * clamp(dot(edge21, pointFrom1) / dot2(edge21)) - pointFrom1),
                                                dot2(edge32 * clamp(dot(edge32, pointFrom2) / dot2(edge32)) - pointFrom2)
                                            ),
                                            dot2(edge13 * clamp(dot(edge13, pointFrom3) / dot2(edge13)) - pointFrom3)
                                        )
                                        :
                                        dot(- normalTrue, pointFrom1) * dot(- normalTrue, pointFrom1) / dot2(- normalTrue)
                                    );
    return distanceMeasure;
}
