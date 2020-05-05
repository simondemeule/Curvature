//
//  MeshPrimitive.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "MeshPrimitive.hpp"

#include "MeshInstance.hpp"

MeshPrimitive::MeshPrimitive(MeshInstance* meshInstance, int indexLocation) :
    ShadableObject(meshInstance->shadableAttributes),
    meshInstance(meshInstance),
    indexLocation(indexLocation)
{
    primitiveCount = 1;
    updateBoundingBox();
}

void MeshPrimitive::updateBoundingBox() {
    for(int i = 0; i < 3; i++) {
        int index = meshInstance->meshData->indices.at(indexLocation + i);
        glm::vec3 corner = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertices.at(index), 1.0));
        if(i == 0) {
            boundingBox.pointPositive = corner;
            boundingBox.pointNegative = corner;
        } else {
            boundingBox.pointPositive = glm::max(corner, boundingBox.pointPositive);
            boundingBox.pointNegative = glm::min(corner, boundingBox.pointNegative);
        }
    }
}

// calculates the area of a triangle given vertex positions
float area(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::length(glm::cross(b - a, c - a)) / 2.0;
}

ShadableObjectIntersection MeshPrimitive::intersection(Ray ray) {
    // get indicies
    int index1 = meshInstance->meshData->indices.at(indexLocation);
    int index2 = meshInstance->meshData->indices.at(indexLocation + 1);
    int index3 = meshInstance->meshData->indices.at(indexLocation + 2);
    
    // get verticies
    glm::vec3 corner1 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertices.at(index1), 1.0));
    glm::vec3 corner2 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertices.at(index2), 1.0));
    glm::vec3 corner3 = glm::vec3(meshInstance->transformation * glm::vec4(meshInstance->meshData->vertices.at(index3), 1.0));
    
    // get normal of triangle plane
    glm::vec3 normal = glm::normalize(glm::cross(corner2 - corner1, corner3 - corner1));
    
    // calculate intersection with triangle plane
    float denom = glm::dot(normal, ray.direction);
    if(denom > -1e-6) {
        // intersection with plane is a backface or ray is parallel to plane
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    float t = glm::dot(corner1 - ray.origin, normal) / denom;
    if(t < 1e-3) {
        // intersection with plane is behind ray
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    
    // intersection with plane is valid
    glm::vec3 v0 = ray.origin + ray.direction * t;
    
    // barycentric area computation
    float a0 = area(corner1, corner2, corner3);
    float a1 = area(v0, corner2, corner3);
    float a2 = area(corner1, v0, corner3);
    float a3 = area(corner1, corner2, v0);
    
    if(std::abs((a1 + a2 + a3) / a0 - 1.0) > 1e-3) {
        // intersection is not in triangle
        // exit calculation
        ShadableObjectIntersection intersection;
        intersection.exists = false;
        return intersection;
    }
    
    // valid intersection, fill in remaining information
    ShadableObjectIntersection intersection;
    intersection.exists = true;
    intersection.origin = ray.origin + ray.direction * t;
    intersection.normal = normal;
    intersection.distance = t;
    intersection.incident = ray.direction;
    intersection.shadableObject = this;
    return intersection;
}
