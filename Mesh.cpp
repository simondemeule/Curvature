//
//  Mesh.cpp
//  project
//
//  Created by Simon Demeule on 2019-03-30.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Mesh.hpp"
#include "OBJloader.h"

Mesh::Mesh(std::string filePath, ShadingAttributes shadingAttributesNew) : ShadingObject(shadingAttributesNew) {
    loadOBJ(filePath.c_str(), indices, vertices, normals, UVs);
    
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

// calculates the area of a triangle
float area(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) / 2.0;
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
            
            glm::vec2 p0; // intersection point projected to plane
            glm::vec2 p1; // vertex 1 projected to plane
            glm::vec2 p2; // vertex 2 projected to plane
            glm::vec2 p3; // vertex 3 projected to plane
            
            // do the projection
            
            // find best projection plane
            if(normal.x > normal.y && normal.x > normal.z) {
                // worse plane is parallel to x
                // choose yz
                p0.x = v0.y;
                p0.y = v0.z;
                p1.x = v1.y;
                p1.y = v1.z;
                p2.x = v2.y;
                p2.y = v2.z;
                p3.x = v3.y;
                p3.y = v3.z;
            } else if(normal.y > normal.x && normal.y > normal.z) {
                // worse plane is parallel to y
                // choose xz
                p0.x = v0.x;
                p0.y = v0.z;
                p1.x = v1.x;
                p1.y = v1.z;
                p2.x = v2.x;
                p2.y = v2.z;
                p3.x = v3.x;
                p3.y = v3.z;
            } else {
                // worse plane is parallel to z
                // choose xy
                p0.x = v0.x;
                p0.y = v0.y;
                p1.x = v1.x;
                p1.y = v1.y;
                p2.x = v2.x;
                p2.y = v2.y;
                p3.x = v3.x;
                p3.y = v3.y;
            }
            
            float a = area(p1, p2, p3); // area for triangle p1-p2-p3
            float c1 = area(p0, p2, p3) / a; // barycentric coordinate for triangle p0-p2-p3
            float c2 = area(p1, p0, p3) / a; // barycentric coordinate for for triangle p1-p0-p3
            float c3 = area(p1, p2, p0) / a; // barycentric coordinate for for triangle p1-p2-p0
            
            if(c1 < -1e-6 ||
               c2 < -1e-6 ||
               c3 < -1e-6 ||
               std::abs(c1 + c2 + c3 - 1.0) > 1e-6) {
                // intersection is not in triangle
                // exit calculation for current triangle
                continue;
            }
            
            // intersection is valid
            if(!intersection.exists || intersection.distance > t) {
                // intersection is the first found or is closer than the one previously found
                
//                // get normals
//                glm::vec3 n1 = glm::normalize(normals[i1]);
//                glm::vec3 n2 = glm::normalize(normals[i2]);
//                glm::vec3 n3 = glm::normalize(normals[i3]);
//                // interpolate normal
//                intersection.normal = c1 * n1 + c2 * n2 + c3 * n3;

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
