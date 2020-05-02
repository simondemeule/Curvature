//
//  Mesh.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Mesh.hpp"
#include "OBJloader.h"

Mesh::Mesh(std::string filePath, ShadingAttributes shadingAttributesNew) : ShadingObject(shadingAttributesNew) {
    loadOBJ(filePath.c_str(), indices, vertices, normals, UVs);
    primitiveCount = indices.size() / 3;
    updateBoundingBox();
}

void Mesh::updateBoundingBox() {
    // scan through vertices to find most positive and most negative coordinates
    glm::vec3 pointPositive = vertices[0];
    glm::vec3 pointNegative = vertices[0];
    for(int i = 1; i < vertices.size(); i++) {
        if(vertices[i].x > pointPositive.x) {
            pointPositive.x = vertices[i].x;
        }
        if(vertices[i].y > pointPositive.y) {
            pointPositive.y = vertices[i].y;
        }
        if(vertices[i].z > pointPositive.z) {
            pointPositive.z = vertices[i].z;
        }
        if(vertices[i].x < pointNegative.x) {
            pointNegative.x = vertices[i].x;
        }
        if(vertices[i].y < pointNegative.y) {
            pointNegative.y = vertices[i].y;
        }
        if(vertices[i].z < pointNegative.z) {
            pointNegative.z = vertices[i].z;
        }
    }
    boundingBox.setPointPositive(pointPositive);
    boundingBox.setPointNegative(pointNegative);
}

// calculates the area of a triangle given vertex positions
float area(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::length(glm::cross(b - a, c - a)) / 2.0;
}

Intersection Mesh::intersection(Ray ray) {
    Intersection intersection;
    if(!boundingBox.intersectionTest(ray)) {
        // no intersection with bounding box
        // measured to speed up render time by over 1200% on a scene with a single mesh with just 24 triangles
        // exit calculation
        intersection.exists = false;
        return intersection;
    } else {
        intersection.exists = false;
        for(int i = 0; i < indices.size() / 3; i++) {
            // get indicies
            int i1 = indices[3 * i];
            int i2 = indices[3 * i + 1];
            int i3 = indices[3 * i + 2];
            
            // get verticies
            glm::vec3 v1 = vertices[i1];
            glm::vec3 v2 = vertices[i2];
            glm::vec3 v3 = vertices[i3];
            
            // get normal of triangle plane
            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            
            // calculate intersection with triangle plane
            float denom = glm::dot(normal, ray.direction);
            if(denom > -1e-6) {
                // intersection with plane is a backface or ray is parallel to plane
                // exit calculation for current triangle
                continue;
            }
            float t = glm::dot(v1 - ray.origin, normal) / denom;
            if(t < 1e-3) {
                // intersection with plane is behind ray
                // exit calculation for current triangle
                continue;
            }
            
            // intersection with plane is valid
            glm::vec3 v0 = ray.origin + ray.direction * t;
            
            // barycentric area computation
            float a0 = area(v1, v2, v3);
            float a1 = area(v0, v2, v3);
            float a2 = area(v1, v0, v3);
            float a3 = area(v1, v2, v0);
            
            if(std::abs((a1 + a2 + a3) / a0 - 1.0) > 1e-3) {
                // intersection is not in triangle
                // exit calculation for current triangle
                continue;
            }
            
            // intersection is valid
            if(!intersection.exists || intersection.distance > t) {
                // intersection is the first found or is closer than the one previously found

                intersection.exists = true;
                intersection.origin = ray.origin + ray.direction * t;
                intersection.normal = normal;
                intersection.distance = t;
            }
            
        }
        
        if(!intersection.exists) {
            // found no intersecting triangle
            return intersection;
        }
        
        // found intersection, fill in remaining information
        intersection.incident = ray.direction;
        intersection.shadingObject = this;
        return intersection;
    }
}
