//
//  RenderData.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-04-06.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "RenderData.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>

#include "Sphere.hpp"
#include "Plane.hpp"
#include "Mesh.hpp"

void RenderData::loadInputFile() {
    std::ifstream file(workingDirectory + inputFileName);
    if(!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }
    
    std::string line;
    
    // skip the first line
    std::getline(file, line);
    // loop through the next lines
    while(std::getline(file, line)) {
        std::istringstream iss(line);
        std::string result;
        if(std::getline(iss, result)) {
            // switch between object types
            if(result == "camera") {
                // parse camera data
                glm::vec3* origin = nullptr;
                float* fieldOfView = nullptr;
                float* focalLength = nullptr;
                float* aspectRatio = nullptr;
                for(int i = 0; i < 4; i++) {
                    std::getline(file, line);
                    iss = std::istringstream(line);
                    std::getline(iss, result, ' ');
                    if(result == "pos:") {
                        delete origin;
                        origin = new glm::vec3();
                        std::getline(iss, result, ' ');
                        origin->x = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->y = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->z = std::stof(result);
                    } else if(result == "fov:") {
                        delete fieldOfView;
                        fieldOfView = new float;
                        std::getline(iss, result, ' ');
                        *fieldOfView = std::stof(result);
                    } else if(result == "f:") {
                        delete focalLength;
                        focalLength = new float;
                        std::getline(iss, result, ' ');
                        *focalLength = std::stof(result);
                    } else if(result == "a:") {
                        delete aspectRatio;
                        aspectRatio = new float;
                        std::getline(iss, result, ' ');
                        *aspectRatio = std::stof(result);
                    }
                }
                if(origin != nullptr &&
                   fieldOfView != nullptr &&
                   focalLength != nullptr &&
                   aspectRatio != nullptr) {
                    delete camera;
                    camera = new Camera(*origin, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), *fieldOfView, *focalLength, *aspectRatio);
                    delete origin;
                    delete fieldOfView;
                    delete focalLength;
                    delete aspectRatio;
                } else {
                    delete origin;
                    delete fieldOfView;
                    delete focalLength;
                    delete aspectRatio;
                    throw std::runtime_error("Input file has incomplete data for camera");
                }
            } else if(result == "plane") {
                // parse plane data
                glm::vec3* origin = nullptr;
                glm::vec3* normal = nullptr;
                glm::vec3* ambient = nullptr;
                glm::vec3* diffuse = nullptr;
                glm::vec3* specular = nullptr;
                float* shininess = nullptr;
                for(int i = 0; i < 6; i++) {
                    std::getline(file, line);
                    iss = std::istringstream(line);
                    std::getline(iss, result, ' ');
                    if(result == "nor:") {
                        delete normal;
                        normal = new glm::vec3();
                        std::getline(iss, result, ' ');
                        normal->x = std::stof(result);
                        std::getline(iss, result, ' ');
                        normal->y = std::stof(result);
                        std::getline(iss, result, ' ');
                        normal->z = std::stof(result);
                    } else if(result == "pos:") {
                        delete origin;
                        origin = new glm::vec3();
                        std::getline(iss, result, ' ');
                        origin->x = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->y = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->z = std::stof(result);
                    } else if(result == "amb:") {
                        delete ambient;
                        ambient = new glm::vec3();
                        std::getline(iss, result, ' ');
                        ambient->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->b = std::stof(result);
                    } else if(result == "dif:") {
                        delete diffuse;
                        diffuse = new glm::vec3();
                        std::getline(iss, result, ' ');
                        diffuse->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->b = std::stof(result);
                    } else if(result == "spe:") {
                        delete specular;
                        specular = new glm::vec3();
                        std::getline(iss, result, ' ');
                        specular->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->b = std::stof(result);
                    } else if(result == "shi:") {
                        delete shininess;
                        shininess = new float;
                        std::getline(iss, result, ' ');
                        *shininess = std::stof(result);
                    }
                }
                if(origin != nullptr &&
                   normal != nullptr &&
                   ambient != nullptr &&
                   diffuse != nullptr &&
                   specular != nullptr &&
                   shininess != nullptr) {
                    ShadableAttributes shadingAttributes;
                    shadingAttributes.ambient = *ambient;
                    shadingAttributes.diffuse = *diffuse;
                    shadingAttributes.specular = *specular;
                    shadingAttributes.shininess = *shininess;
                    objects.push_back(new Plane(*origin, *normal, shadingAttributes));
                    delete origin;
                    delete normal;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                } else {
                    delete origin;
                    delete normal;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                    throw std::runtime_error("Input file has incomplete data for plane");
                }
            } else if(result == "sphere") {
                // parse sphere data
                glm::vec3* origin = nullptr;
                float* radius = nullptr;
                glm::vec3* ambient = nullptr;
                glm::vec3* diffuse = nullptr;
                glm::vec3* specular = nullptr;
                float* shininess = nullptr;
                for(int i = 0; i < 6; i++) {
                    std::getline(file, line);
                    iss = std::istringstream(line);
                    std::getline(iss, result, ' ');
                    if(result == "pos:") {
                        delete origin;
                        origin = new glm::vec3();
                        std::getline(iss, result, ' ');
                        origin->x = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->y = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->z = std::stof(result);
                    } else if(result == "rad:") {
                        delete radius;
                        radius = new float;
                        std::getline(iss, result, ' ');
                        *radius = std::stof(result);
                    } else if(result == "amb:") {
                        delete ambient;
                        ambient = new glm::vec3();
                        std::getline(iss, result, ' ');
                        ambient->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->b = std::stof(result);
                    } else if(result == "dif:") {
                        delete diffuse;
                        diffuse = new glm::vec3();
                        std::getline(iss, result, ' ');
                        diffuse->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->b = std::stof(result);
                    } else if(result == "spe:") {
                        delete specular;
                        specular = new glm::vec3();
                        std::getline(iss, result, ' ');
                        specular->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->b = std::stof(result);
                    } else if(result == "shi:") {
                        delete shininess;
                        shininess = new float;
                        std::getline(iss, result, ' ');
                        *shininess = std::stof(result);
                    }
                }
                if(origin != nullptr &&
                   radius != nullptr &&
                   ambient != nullptr &&
                   diffuse != nullptr &&
                   specular != nullptr &&
                   shininess != nullptr) {
                    ShadableAttributes shadingAttributes;
                    shadingAttributes.ambient = *ambient;
                    shadingAttributes.diffuse = *diffuse;
                    shadingAttributes.specular = *specular;
                    shadingAttributes.shininess = *shininess;
                    objects.push_back(new Sphere(*origin, *radius, shadingAttributes));
                    delete origin;
                    delete radius;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                } else {
                    delete origin;
                    delete radius;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                    throw std::runtime_error("Input file has incomplete data for sphere");
                }
            } else if(result == "mesh") {
                // parse mesh data
                std::string* meshFileName = nullptr;
                glm::vec3* ambient = nullptr;
                glm::vec3* diffuse = nullptr;
                glm::vec3* specular = nullptr;
                float* shininess = nullptr;
                for(int i = 0; i < 5; i++) {
                    std::getline(file, line);
                    iss = std::istringstream(line);
                    std::getline(iss, result, ' ');
                    if(result == "file:") {
                        delete meshFileName;
                        meshFileName = new std::string;
                        std::getline(iss, result, ' ');
                        *meshFileName = result;
                    } else if(result == "amb:") {
                        delete ambient;
                        ambient = new glm::vec3();
                        std::getline(iss, result, ' ');
                        ambient->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        ambient->b = std::stof(result);
                    } else if(result == "dif:") {
                        delete diffuse;
                        diffuse = new glm::vec3();
                        std::getline(iss, result, ' ');
                        diffuse->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->b = std::stof(result);
                    } else if(result == "spe:") {
                        delete specular;
                        specular = new glm::vec3();
                        std::getline(iss, result, ' ');
                        specular->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->b = std::stof(result);
                    } else if(result == "shi:") {
                        delete shininess;
                        shininess = new float;
                        std::getline(iss, result, ' ');
                        *shininess = std::stof(result);
                    }
                }
                if(meshFileName != nullptr &&
                   ambient != nullptr &&
                   diffuse != nullptr &&
                   specular != nullptr &&
                   shininess != nullptr) {
                    ShadableAttributes shadingAttributes;
                    shadingAttributes.ambient = *ambient;
                    shadingAttributes.diffuse = *diffuse;
                    shadingAttributes.specular = *specular;
                    shadingAttributes.shininess = *shininess;
                    
                    objects.push_back(new Mesh(workingDirectory + *meshFileName, shadingAttributes));
                    delete meshFileName;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                } else {
                    delete meshFileName;
                    delete ambient;
                    delete diffuse;
                    delete specular;
                    delete shininess;
                    throw std::runtime_error("Input file has incomplete data for mesh");
                }
            } else if(result == "light") {
                // parse light data
                glm::vec3* origin = nullptr;
                glm::vec3* diffuse = nullptr;
                glm::vec3* specular = nullptr;
                for(int i = 0; i < 3; i++) {
                    std::getline(file, line);
                    iss = std::istringstream(line);
                    std::getline(iss, result, ' ');
                    if(result == "pos:") {
                        delete origin;
                        origin = new glm::vec3();
                        std::getline(iss, result, ' ');
                        origin->x = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->y = std::stof(result);
                        std::getline(iss, result, ' ');
                        origin->z = std::stof(result);
                    } else if(result == "dif:") {
                        delete diffuse;
                        diffuse = new glm::vec3();
                        std::getline(iss, result, ' ');
                        diffuse->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        diffuse->b = std::stof(result);
                    } else if(result == "spe:") {
                        delete specular;
                        specular = new glm::vec3();
                        std::getline(iss, result, ' ');
                        specular->r = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->g = std::stof(result);
                        std::getline(iss, result, ' ');
                        specular->b = std::stof(result);
                    }
                }
                if(diffuse != nullptr &&
                   specular != nullptr) {
                    ShadableAttributes shadingAttributes;
                    shadingAttributes.diffuse = *diffuse;
                    shadingAttributes.specular = *specular;
                    lights.push_back(new Light(*origin, shadingAttributes));
                    delete origin;
                    delete diffuse;
                    delete specular;
                } else {
                    delete origin;
                    delete diffuse;
                    delete specular;
                    throw std::runtime_error("Input file has incomplete data for light");
                }
            }
        }
    }
    
    file.close();
}

void RenderData::loadTest() {
    ShadableAttributes shadingAttributes;
    shadingAttributes.ambient = glm::vec3(0.1);
    shadingAttributes.diffuse = glm::vec3(0.8);
    shadingAttributes.specular = glm::vec3(0.8);
    shadingAttributes.shininess = 1;
    shadingAttributes.reflectivity = 0.2;
    
    // sphere grid
    for(int i = -3; i <= 3; i++) {
        for(int j = -3; j <= 3; j++) {
            objects.push_back(new Sphere(glm::vec3(i * 2, j * 2, 0), 1, shadingAttributes));
        }
    }
    
    // plane
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.0, 0.9, 0.6);
    shadingAttributes.reflectivity = 0;
    //objects.push_back(new Plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), shadingAttributes));
    
    // mesh
    //objects.push_back(new Mesh("monkey.obj", shadingAttributes));
    
    // sphere floating miror
    shadingAttributes.ambient = glm::vec3(0);
    shadingAttributes.diffuse = glm::vec3(0);
    shadingAttributes.specular = glm::vec3(0);
    shadingAttributes.shininess = 0;
    shadingAttributes.reflectivity = 1;
    objects.push_back(new Sphere(glm::vec3(0, 0, 4), 2, shadingAttributes));
    
    // sphere over camera
    //objects.push_back(new Sphere(glm::vec3(0, -4, 2), 1, shadingAttributes));
    
    // some other sphere
    //objects.push_back(new Sphere(glm::vec3(1, -2, 4), 1, shadingAttributes));
    
    // lights
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.9, 0.0, 0.6);
    lights.push_back(new Light(glm::vec3(4, -4, 2), shadingAttributes));
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.3, 0.1, 0.9);
    lights.push_back(new Light(glm::vec3(-4, 4, 2), shadingAttributes));
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.0, 0.9, 0.6);
    lights.push_back(new Light(glm::vec3(-4, 4, 20), shadingAttributes));
    // camera
    camera = new Camera(glm::vec3(0, -4, 2), glm::vec3(0, 4, -2), glm::vec3(0, 0, 1), 80, 1.0, 16.0 / 9.0);
    //camera = new Camera(glm::vec3(0, 0, 2), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 80, 1.0, 16.0 / 9.0);
}

void RenderData::applyOutputSettingsFromFocalPlane() {
    applyOutputSettingsFromWidth(2.0 * std::tanf(camera->fieldOfView / 360.0 * M_PI) * camera->aspectRatio * camera->focalLength);
}

void RenderData::applyOutputSettingsFromWidth(int outputWidthNew) {
    outputWidth = outputWidthNew;
    outputHeight = outputWidthNew / camera->aspectRatio;
}

void RenderData::applyOutputSettingsFromHeight(int outputHeightNew) {
    outputWidth = outputHeightNew * camera->aspectRatio;
    outputHeight = outputHeightNew;
}

void RenderData::applyTileSettings(int tileSizeNew) {
    tileSize = tileSizeNew;
    tileCountWidth = (outputWidth + tileSize - 1) / tileSize;   // tileCountWidth = ceil(outputWidth / tileSize)
    tileCountHeight = (outputHeight + tileSize - 1) / tileSize; // tileCountHeight = ceil(outputHeight / tileSize)
    tileCount = tileCountWidth * tileCountHeight;
}

void RenderData::applyAntiAliasingSettings(int antiAliasingPassesRootNew) {
    antiAliasingPassesRoot = antiAliasingPassesRootNew;
    antiAliasingPasses = antiAliasingPassesRoot * antiAliasingPassesRoot;
}

void RenderData::applyThreadSettings(int threadCountNew) {
    threadCount = threadCountNew;
}

void RenderData::applyThreadSettings() {
    threadCount = std::thread::hardware_concurrency();
    if(threadCount <= 0) {
        // this will happen if the above fails to detect the number of threads supported
        threadCount = 1;
    }
}

void RenderData::applyRecursionSettings(int recursionLimitNew) {
    recursionLimit = recursionLimitNew;
}

void RenderData::computeBoundedHierarchy() {
    std::cout << "Building BVH tree" << std::endl;
    boundedHierarchy = new BoundedHierarchy(objects);
}

// constructor for tests with fancy features
RenderData::RenderData(int outputWidthNew, int tileSizeNew, int antiAliasingPassesRootNew, int threadCountNew, int recursionLimitNew, std::string workingDirectoryNew, std::string outputFileNameNew) {
    workingDirectory = workingDirectoryNew;
    outputFileName = outputFileNameNew;
    
    loadTest();

    applyOutputSettingsFromWidth(outputWidthNew);
    applyTileSettings(tileSizeNew);
    applyAntiAliasingSettings(antiAliasingPassesRootNew);
    applyThreadSettings(threadCountNew);
    applyRecursionSettings(recursionLimitNew);
    computeBoundedHierarchy();
}

// constructor for assignment files
RenderData::RenderData(std::string workingDirectoryNew, std::string inputFileNameNew, std::string outputFileNameNew) {
    workingDirectory = workingDirectoryNew;
    inputFileName = inputFileNameNew;
    outputFileName = outputFileNameNew;
    
    loadInputFile();
    
    applyOutputSettingsFromFocalPlane();
    applyTileSettings(64);
    applyAntiAliasingSettings(1);
    applyThreadSettings();
    applyRecursionSettings(0);
    computeBoundedHierarchy();
}

// calculate camera coordinates for given pixel and anti aliasing pass
// input:
// x -> [0, outputWidth]
// y -> [0, outputHeight]
// output:
// x -> [-1, -1]
// y -> [-1 / aspectRatio, 1 / aspectRatio]

// TODO: implement better aliasing with an actual filter
// https://computergraphics.stackexchange.com/questions/2130/anti-aliasing-filtering-in-ray-tracing
glm::vec2 RenderData::toNormalizedCoordinates(int x, int y, int antiAliasingPass) {
    float xOffset;
    float yOffset;
    if(antiAliasingPassesRoot == 1) {
        xOffset = 0.5;
        yOffset = 0.5;
    } else {
        float span = 1.0 / antiAliasingPassesRoot;
        xOffset = span * (0.5 + antiAliasingPass % antiAliasingPassesRoot);
        yOffset = span * (0.5 + antiAliasingPass / antiAliasingPassesRoot);
    }
    return glm::vec2((x + xOffset - outputWidth / 2.0) / outputWidth * 2.0,
                     - (y + yOffset - outputHeight / 2.0) / outputWidth * 2.0);
}

// Intersection closestIntersectionThroughPortal(Ray incoming, Portal* portal)

// Intersection closestIntersectionThroughField(Ray incoming, Field* field, float step, int recursionDepth)
//
// calculate outgoing ray
// check if calculated ray respects max step size
// -> does not respect max step size
//    recursively recalculate
// calculate quantized grid center positions that might be crossed (including incoming origin)
// check quantized spatial tree for objects at those positions
// -> objects found
//    calculate closest intersection from incoming ray with the found objects
//    check if intersection distance < incoming to outgoing distance
//    -> smaller
//       return closest intersection
// check if outgoing ray origin is within field
// -> within field
//    check if recursion depth exceeded
//    -> depth exceeded
//       return no intersection
//    -> depth not exceeded
//       recursively march field
// -> outisde field
//    return closest intersection from outgoing ray with all objects

// ======================================================================================================

// notes
//
// what the hell happens when two fields overlap?
//   maybe just disallow it. it's easier to write an extension of the field class that's actually multiple fields.
//   maybe just sum the delta from the incoming ray? means changing the way the marching function operates.
//
// need to update intersection type to allow intersections with fields (also implies redesigning hierarchy for objects, etc.)
//
// this (method described above) is called once we know a ray intersects with a field
//
// rays can enter fields by either intersecting with them or by containment (having their origin in them, ie reflected rays of an object within a field)
//
// the spatial data structure needs the folowing
//   all bounding boxes are rounded to multiples of the marching stepsize
//   search by ray intersection for shadable objects
//   search by ray intersection for portals
//   search by ray intersection or containement for fields
//   search by multiple position containement for shadable objects (returns those that contains any of the points, not those that contain all of the points)

// ======================================================================================================

// ideas
//
// The idea behind the quantized approach is that if the marching step is guaranteed to be smaller than or equal to the quantization step of the spatial data structure,
// a marching ray is entirely confined in a 3 x 3 x 3 quantized cube. With some comparaison operations can narrow it down to a 2 x 2 x 2
// cube in the worse case, and a 1 x 1 x 1 cube in the best case.
//
// If there are no object whose bounding boxes occupy this volume, (which is very often) there cannot be an intersection,
// and we can move on to the next marching step without computing intersections
//
// If everything is quantized to the stepsize, why not do all the bounding box / spatial data structure math with integers too!

// newer ideas
//
// make the field method return a ray delta rather than a ray so that multiple fields can intersect
// then intersection methods shouldnt be per portal / per field, must rethink that

// ======================================================================================================

// even newer ideas
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

// calculate closest intersection between ray and all objects
ShadableObjectIntersection RenderData::closestIntersection(Ray ray) {
    return boundedHierarchy->closestIntersection(ray);
}

// calculate the color contribution of a light to an intersection
glm::vec3 RenderData::colorLight(ShadableObjectIntersection intersection, Light* light) {
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
        color += glm::dot(lightRay.direction, intersection.normal) * intersection.shadableObject->shadingAttributes.diffuse * light->shadingAttributes.diffuse;
        // specular
        color += std::pow(std::fmaxf(0, glm::dot(lightRay.direction, glm::reflect(intersection.incident, intersection.normal))), intersection.shadableObject->shadingAttributes.shininess) * intersection.shadableObject->shadingAttributes.specular * light->shadingAttributes.specular;
    }
    return color;
}

// calculate the color of an intersection
glm::vec3 RenderData::colorIntersection(ShadableObjectIntersection intersection, int recursionDepth) {
    glm::vec3 color(0.0);
    for(int i = 0; i < lights.size(); i++) {
        // diffuse and specular (taking visibility into account)
        color += colorLight(intersection, lights[i]);
    }
    // ambient
    color += intersection.shadableObject->shadingAttributes.ambient;
    // recursive reflection
    if(recursionDepth > 0) {
        Ray reflected;
        reflected.origin = intersection.origin;
        reflected.direction = glm::reflect(intersection.incident, intersection.normal);
        reflected.isCameraRay = true;
        ShadableObjectIntersection closest = closestIntersection(reflected);
        if(closest.exists) {
            color += intersection.shadableObject->shadingAttributes.reflectivity * colorIntersection(closest, recursionDepth - 1);
        }
    }
    return color;
}

// calculate the color of a ray
glm::vec3 RenderData::colorRay(Ray ray) {
    ShadableObjectIntersection closest = closestIntersection(ray);
    if(closest.exists) {
        return colorIntersection(closest, recursionLimit);
    } else {
        return glm::vec3(0);
    }
}
