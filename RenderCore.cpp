//
//  RenderCore.cpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#include "RenderCore.hpp"

RenderCore::RenderCore(RenderData* renderData) : renderData(renderData) {}

// calculate camera coordinates for given pixel and anti aliasing pass
// input:
// x -> [0, outputWidth]
// y -> [0, outputHeight]
// output:
// x -> [-1, -1]
// y -> [-1 / aspectRatio, 1 / aspectRatio]

// TODO: implement better aliasing with an actual filter
// https://computergraphics.stackexchange.com/questions/2130/anti-aliasing-filtering-in-ray-tracing
glm::vec2 RenderCore::toNormalizedCoordinates(int x, int y, int antiAliasingPass) {
    float xOffset;
    float yOffset;
    if(renderData->antiAliasingPassesRoot == 1) {
        xOffset = 0.5;
        yOffset = 0.5;
    } else {
        float span = 1.0 / renderData->antiAliasingPassesRoot;
        xOffset = span * (0.5 + antiAliasingPass % renderData->antiAliasingPassesRoot);
        yOffset = span * (0.5 + antiAliasingPass / renderData->antiAliasingPassesRoot);
    }
    return glm::vec2((x + xOffset - renderData->outputWidth / 2.0) / renderData->outputWidth * 2.0,
                   - (y + yOffset - renderData->outputHeight / 2.0) / renderData->outputWidth * 2.0);
}

// calculate closest intersection between ray and all objects
ShadableObjectIntersection RenderCore::closestIntersection(Ray ray) {
    return renderData->boundedHierarchy->closestIntersection(ray);
}

// calculate the color contribution of a light to an intersection
glm::vec3 RenderCore::colorLight(ShadableObjectIntersection intersection, Light* light) {
    glm::vec3 color(0.0);
    
    Ray lightRay;
    lightRay.origin = intersection.origin;
    lightRay.direction = light->origin - intersection.origin;
    float distance = glm::length(lightRay.direction);
    lightRay.direction = glm::normalize(lightRay.direction);
    lightRay.isCameraRay = false;
    
    ShadableObjectIntersection closest = closestIntersection(lightRay);
    if(!closest.exists || closest.distance > distance) {
        // light ray is unobstructed, calculate contribution
        // diffuse
        color += glm::dot(lightRay.direction, intersection.normal) * intersection.shadableObject->shadableAttributes->diffuse * light->shadableAttributes.diffuse;
        // specular
        color += std::pow(std::fmaxf(0, glm::dot(lightRay.direction, glm::reflect(intersection.incident, intersection.normal))), intersection.shadableObject->shadableAttributes->shininess) * intersection.shadableObject->shadableAttributes->specular * light->shadableAttributes.specular;
    }
    return color;
}

// calculate the color of an intersection
glm::vec3 RenderCore::colorIntersection(ShadableObjectIntersection intersection, int recursionDepth) {
    glm::vec3 color(0.0);
    for(int i = 0; i < renderData->lights.size(); i++) {
        // diffuse and specular (taking visibility into account)
        color += colorLight(intersection, renderData->lights[i]);
    }
    // ambient
    color += intersection.shadableObject->shadableAttributes->ambient;
    // recursive reflection
    if(recursionDepth > 0) {
        Ray reflected;
        reflected.origin = intersection.origin;
        reflected.direction = glm::reflect(intersection.incident, intersection.normal);
        reflected.isCameraRay = true;
        ShadableObjectIntersection closest = closestIntersection(reflected);
        if(closest.exists) {
            color += intersection.shadableObject->shadableAttributes->reflectivity * colorIntersection(closest, recursionDepth - 1);
        }
    }
    return color;
}

DistanceMeasure RenderCore::marchDistanceRay(Ray ray) {
    DistanceMeasure distanceMeasure = renderData->boundedHierarchy->distance(ray.origin);
    if(distanceMeasure.distance > 0.02 && distanceMeasure.distance < 20) {
        Ray rayRecursive = ray;
        rayRecursive.origin = ray.origin + ray.direction * distanceMeasure.distance * 0.999f;
        DistanceMeasure distanceMeasureRecursive = marchDistanceRay(rayRecursive);
        DistanceMeasure distanceMeasureFinal;
        distanceMeasureFinal.distance = distanceMeasure.distance + distanceMeasureRecursive.distance;
        distanceMeasureFinal.boxDistanceDepth = distanceMeasureRecursive.boxDistanceDepth;
        distanceMeasureFinal.objectDistanceDepth = distanceMeasureRecursive.objectDistanceDepth;
        distanceMeasureFinal.debugMarker = distanceMeasureRecursive.debugMarker;
        return distanceMeasureFinal;
    } else {
        return distanceMeasure;
    }
}

// calculate the color of a ray
glm::vec3 RenderCore::colorRay(Ray ray) {
    //ShadableObjectIntersection intersection = closestIntersection(ray);
    //float castDistance = intersection.exists ? intersection.distance : 100.0;
    //DistanceMeasure marchDistance = marchDistanceRay(ray);
    //return glm::vec3(marchDistance.distance / 10.0, marchDistance.objectDistanceDepth / 10.0, marchDistance.boxDistanceDepth / 10.0);
    
    ShadableObjectIntersection closest = closestIntersection(ray);
    //return glm::vec3(closest.exists ? closest.distance / 10.0 : 100.0, closest.debugMarker, closest.boxIntersectionDepth / 40.0);
    //return glm::vec3(closest.objectIntersectionDepth / 255.0, closest.boxIntersectionDepth / 255.0, 0.0);
    
    if(closest.exists) {
        return colorIntersection(closest, renderData->recursionLimit);
        //return 0.5f * (closest.normal + 1.0f);
    } else {
        return glm::vec3(0);
    }
}

// calculate the color of a pixel
glm::vec3 RenderCore::colorPixel(int x, int y) {
    glm::vec3 color(0.0);
    // accumulate anti-aliasing passes
    for(int a = 0; a < renderData->antiAliasingPasses; a++) {
        glm::vec2 normalizedCoordinates = toNormalizedCoordinates(x, y, a);
        Ray ray = renderData->camera->castNormalized(normalizedCoordinates);
        color += colorRay(ray);
    }
    // average passes
    color *= 1.0 / renderData->antiAliasingPasses;
    return color;
}
