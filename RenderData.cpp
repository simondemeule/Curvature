//
//  RenderData.cpp
//  project
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
                    ShadingAttributes shadingAttributes;
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
                    ShadingAttributes shadingAttributes;
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
                    ShadingAttributes shadingAttributes;
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
                    ShadingAttributes shadingAttributes;
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
    ShadingAttributes shadingAttributes;
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
    objects.push_back(new Plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), shadingAttributes));
    
    // sphere floating miror
    shadingAttributes.ambient = glm::vec3(0);
    shadingAttributes.diffuse = glm::vec3(0);
    shadingAttributes.specular = glm::vec3(0);
    shadingAttributes.shininess = 0;
    shadingAttributes.reflectivity = 1;
    objects.push_back(new Sphere(glm::vec3(0, 0, 4), 2, shadingAttributes));
    
    // sphere over camera
    objects.push_back(new Sphere(glm::vec3(0, -4, 2), 1, shadingAttributes));
    
    // lights
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.9, 0.0, 0.6);
    lights.push_back(new Light(glm::vec3(4, -4, 2), shadingAttributes));
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.3, 0.1, 0.9);
    lights.push_back(new Light(glm::vec3(-4, 4, 2), shadingAttributes));
    shadingAttributes.specular = shadingAttributes.diffuse = glm::vec3(0.0, 0.9, 0.6);
    lights.push_back(new Light(glm::vec3(-4, 4, 20), shadingAttributes));
    // camera
    camera = new Camera(glm::vec3(0, -4, 2), glm::vec3(0, 4, -2), glm::vec3(0, 0, 1), 80, 1.0, 16.0 / 9.0);
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
}

// calculate camera coordinates for given pixel and anti aliasing pass
// input:
// x -> [0, outputWidth]
// y -> [0, outputHeight]
// output:
// x -> [-1, -1]
// y -> [-1 / aspectRatio, 1 / aspectRatio]
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

// calculate closest intersection between ray and all objects
Intersection RenderData::closestIntersection(Ray ray) {
    Intersection closest;
    closest.exists = false;
    for(int i = 0; i < objects.size(); i++) {
        Intersection temp = objects[i]->intersection(ray);
        if(temp.exists && (!closest.exists || temp.distance < closest.distance)) {
            closest = temp;
        }
    }
    return closest;
}

// calculate the color contribution of a light to an intersection
glm::vec3 RenderData::colorLight(Intersection intersection, Light* light) {
    glm::vec3 color(0.0);
    
    Ray lightRay;
    lightRay.origin = intersection.origin;
    lightRay.direction = light->origin - intersection.origin;
    float distance = glm::length(lightRay.direction);
    lightRay.direction = glm::normalize(lightRay.direction);
    lightRay.isCameraRay = false;
    
    Intersection closest = closestIntersection(lightRay);
    if(!closest.exists || closest.distance > distance) {
        // light ray is unobstructed, calculate contribution
        // diffuse
        color += glm::dot(lightRay.direction, intersection.normal) * intersection.shadingObject->shadingAttributes.diffuse * light->shadingAttributes.diffuse;
        // specular
        color += std::pow(std::fmaxf(0, glm::dot(lightRay.direction, glm::reflect(intersection.incident, intersection.normal))), intersection.shadingObject->shadingAttributes.shininess) * intersection.shadingObject->shadingAttributes.specular * light->shadingAttributes.specular;
    }
    return color;
}

// calculate the color of an intersection
glm::vec3 RenderData::colorIntersection(Intersection intersection, int recursionDepth) {
    glm::vec3 color(0.0);
    for(int i = 0; i < lights.size(); i++) {
        // diffuse and specular (taking visibility into account)
        color += colorLight(intersection, lights[i]);
    }
    // ambient
    color += intersection.shadingObject->shadingAttributes.ambient;
    // recursive reflection
    if(recursionDepth > 0) {
        Ray reflected;
        reflected.origin = intersection.origin;
        reflected.direction = glm::reflect(intersection.incident, intersection.normal);
        reflected.isCameraRay = true;
        Intersection closest = closestIntersection(reflected);
        if(closest.exists) {
            color += intersection.shadingObject->shadingAttributes.reflectivity * colorIntersection(closest, recursionDepth - 1);
        }
    }
    return color;
}

// calculate the color of a ray
glm::vec3 RenderData::colorRay(Ray ray) {
    Intersection closest = closestIntersection(ray);
    if(closest.exists) {
        return colorIntersection(closest, recursionLimit);
    } else {
        return glm::vec3(0);
    }
}
