//
//  RenderCore.hpp
//  Bent
//
//  Created by Simon Demeule on 2020-05-05.
//  Copyright © 2020 Simon Demeule. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

#include "RenderData.hpp"
#include "Ray.hpp"
#include "ShadableObjectIntersection.hpp"
#include "DistanceMeasure.hpp"
#include "Light.hpp"

// class holding all high-level methods used to render a scene. used by RenderThread
class RenderCore {
private:
    RenderData* renderData;
public:
    // constructor
    RenderCore(RenderData* renderData);
    
    // transforms a pixel coordinate and antialiasing pass number to normalized camera plane coordinates
    // input:
    // x -> [0, outputWidth]
    // y -> [0, outputHeight]
    // output:
    // x -> [-1, 1]
    // y -> [-1 / aspectRatio, 1 / aspectRatio]
    glm::vec2 toNormalizedCoordinates(int x, int y, int antiAliasingPass);
    
    // calculate closest intersection between ray and any object
    ShadableObjectIntersection closestIntersection(Ray ray);
    
    // calculate the color contribution of a light to an intersection
    glm::vec3 colorLight(ShadableObjectIntersection intersection, Light* light);
    
    // calculate the color of an intersection
    glm::vec3 colorIntersection(ShadableObjectIntersection intersection, int recursionDepth);
    
    // march ray distance
    DistanceMeasure marchDistanceRay(Ray ray);
    
    // calculate the color of a ray
    glm::vec3 colorRay(Ray ray);
    
    // calculate the color of a pixel
    glm::vec3 colorPixel(int x, int y);
};

// ======================================================================================================

// ideas
//
// have an Intersection class that is the parent of FieldIntersection, ObjectIntersection, PortalIntersection
//
// fuck composite bounding boxes and field groups, they lead to complicated program design and unefficient ray containment checking.
// it's easier to do it individually for each field, because if the ray start and end are both in the bounding box, then we know for sure the ray is entirely in the bounding box.
// this is not true for composite bounding boxes; it has to be heavily manipulated to become a set of non-overlapping boxes that cover the whole composite volume together.
//


// glm::vec3 colorRay(Ray ray)
// color the ray

// compute closest object intersection
// compute closest portal intersection
// compute closest field intersection
// if the field intersection is the closest existing thing
//   set the position of the ray to the field intersection's position, and start marching the field
// else if the portal intersection is the closest existing thing
//   set the position of the ray to the portal's exit and re-orient the ray relative to the portal, and cast the ray again
// else if the object intersection is the closest existing thing
//   color the ray the usual way
// else
//   the ray meets nothing, set the color to black

// glm::vec3 colorObjectIntersection(ObjectIntersection objectIntersection)
// color the ray according to object intersection data

// ObjectIntersection closestObjectIntersection(Ray ray)
// compute closest object intersection

// PortalIntersection closestPortalIntersection(Ray ray)
// compute closest portal intersection

// FieldIntersection closestFieldIntersection(Ray ray)
// compute closest field intersection

// std::vector<Field*> encompassingFields(Ray ray)
// find the fields whose bounding boxes contain the ray origin

// glm::vec3 colorRayThroughFields(Ray ray, std::vector<Fields*>)
// march through fields until exit or intersection with object

// compute contributions from all fields and normalize delta to get next ray origin
// (optional: do a better job than Euler's method, use adaptive Runge-Kutta integration to get better precision and optimal step size)
// create bounding box using old and new ray origins
// search for object or portal which has a bounding box that intersects the ray bounding box (return a vector of portals and a vector of objects)
// if no intersection or intersection at distance greater than increment
//   compute encompassing fields for new ray origin
//   if there are no encompassing fields
//     call colorRay
//   else
//     call recursion
// else
//   compute proper intersection
//   if intersection is portal
//      go through portal
//   else
//      color object the usual way

// glm::vec3 colorRayThroughPortal(Ray ray, Portal* portal)
// make a ray go through a portal

// teleport the ray
// compute encompassing fields for new ray origin
// if there are no encompassing fields
//   call colorRay
// else
//   call colorRayThroughFields

// ======================================================================================================

// more thoughts

// ray-marching can likely be improved by dynamically changing the increment. there probably are integration techniques that do this. might need the derivative of the field for this.
// https://en.wikipedia.org/wiki/Runge–Kutta_methods
// https://en.wikipedia.org/wiki/Runge–Kutta_methods#Adaptive_Runge–Kutta_methods

// dp/dt = f(t, p)
// p(t_o) = p_0

// ======================================================================================================

// even more thoughts

// it's much, much easier to make this work for a scene that is just an HDRI sphere texture map, where nothing intersects with the rays, and we are only interested in the direction of the rays once the exit the field.
// we can just render everything to an image containing the UV data, and map the texture afterward, so that we may reposition everything after the render is done.

// if we really want 3D objects that intersect with the fields, raymarching may be better suited to this. when the distance radius is large enough we can make multiple steps per distance check, as long as the distance between the moment where we made the check and the last step we make is smaller than the distance reported at the check. This really seems like the simplest, most elegant way to accelerate this.
// BVH could be implemented for raymarching: start at the root, sort the children nodes by proximity, recur on the closest one: if an intersection is found at a distance closer than its sibling node, there is no need to recur on it. if the intersection is farther than its sibling distance (shortest path), then recursion is also needed for the sibling.

// info about raymarching distance functions: https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
