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
#include <stdexcept>
#include <thread>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Sphere.hpp"
#include "MeshData.hpp"
#include "MeshInstance.hpp"
#include "FieldCrossProduct.hpp"

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
                    ShadableAttributes* shadableAttributes = new ShadableAttributes();
                    shadableAttributes->ambient = *ambient;
                    shadableAttributes->diffuse = *diffuse;
                    shadableAttributes->specular = *specular;
                    shadableAttributes->shininess = *shininess;
                    objects.push_back(new Sphere(*origin, *radius, shadableAttributes));
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
                    ShadableAttributes* shadableAttributes = new ShadableAttributes();
                    shadableAttributes->ambient = *ambient;
                    shadableAttributes->diffuse = *diffuse;
                    shadableAttributes->specular = *specular;
                    shadableAttributes->shininess = *shininess;
                    
                    MeshData* meshData = new MeshData(workingDirectory + *meshFileName);
                    meshDatas.push_back(meshData);
                    MeshInstance* meshInstance = new MeshInstance(meshData, glm::mat4x4(1.0), shadableAttributes);
                    meshInstances.push_back(meshInstance);
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
                    ShadableAttributes shadableAttributes;
                    shadableAttributes.diffuse = *diffuse;
                    shadableAttributes.specular = *specular;
                    lights.push_back(new Light(*origin, shadableAttributes));
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
    ShadableAttributes* sphereAttributes = new ShadableAttributes();
    sphereAttributes->ambient = glm::vec3(0.1);
    sphereAttributes->diffuse = glm::vec3(0.8);
    sphereAttributes->specular = glm::vec3(0.8);
    sphereAttributes->shininess = 1;
    sphereAttributes->reflectivity = 0.2;
    
    // sphere grid
    for(int i = -3; i <= 3; i++) {
        for(int j = -3; j <= 3; j++) {
            objects.push_back(new Sphere(glm::vec3(i * 2, j * 2, -2), 1, sphereAttributes));
        }
    }
    
    // field
    Field* fieldCrossProduct = new FieldCrossProduct(glm::vec3(0), glm::vec3(0, 0, 1), 3, 1);
    fields.push_back(fieldCrossProduct);
    
    // mesh
    /*
    MeshData* meshData = new MeshData("monkey2.obj");
    meshDatas.push_back(meshData);
    //MeshInstance* meshInstance = new MeshInstance(meshData, glm::rotate(glm::mat4(2.0), (float) M_PI_2, glm::vec3(1.0, 0.0, 0.0)), sphereAttributes);
    MeshInstance* meshInstance = new MeshInstance(meshData,glm::mat4(1.0), sphereAttributes);
    meshInstances.push_back(meshInstance);
    */
    //objects.push_back(new Sphere(glm::vec3(0, 0, 0), 1, sphereAttributes));
    
    // sphere floating miror
    /*
    ShadableAttributes* mirrorAttributes = new ShadableAttributes();
    mirrorAttributes->ambient = glm::vec3(0);
    mirrorAttributes->diffuse = glm::vec3(0);
    mirrorAttributes->specular = glm::vec3(0);
    mirrorAttributes->shininess = 0;
    mirrorAttributes->reflectivity = 1;
    objects.push_back(new Sphere(glm::vec3(0, 0, 4), 2, mirrorAttributes));
    */
    
    // sphere over camera
    //objects.push_back(new Sphere(glm::vec3(0, -4, 2), 1, shadableAttributes));
    
    // some other sphere
    //objects.push_back(new Sphere(glm::vec3(1, -2, 4), 1, shadableAttributes));
    
    // lights
    ShadableAttributes lightAttributes;
    lightAttributes.specular = lightAttributes.diffuse = glm::vec3(0.9, 0.0, 0.6);
    lights.push_back(new Light(glm::vec3(4, -4, 2), lightAttributes));
    lightAttributes.specular = lightAttributes.diffuse = glm::vec3(0.3, 0.1, 0.9);
    lights.push_back(new Light(glm::vec3(-4, 4, 2), lightAttributes));
    lightAttributes.specular = lightAttributes.diffuse = glm::vec3(0.0, 0.9, 0.6);
    lights.push_back(new Light(glm::vec3(-4, 4, 20), lightAttributes));
    // camera
    //camera = new Camera(glm::vec3(0, -4, 0), glm::vec3(0, 4, 0), glm::vec3(0, 0, 1), 80, 1.0, 16.0 / 9.0);
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

void RenderData::computeShadableBoundedHierarchy() {
    std::cout << "Building shadable bounded hierarchy" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    shadableBoundedHierarchy = new BoundedHierarchy<ShadableObject, ShadableObjectIntersection>(objects);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Done in "<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << std::endl;
}

void RenderData::computeFieldBoundedHierarchy() {
    std::cout << "Building field bounded hierarchy" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    fieldBoundedHierarchy = new BoundedHierarchy<Field, FieldIntersection>(fields);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Done in "<< std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << std::endl;
}

void RenderData::expandPrimitives() {
    for(int i = 0; i < meshInstances.size(); i++) {
        std::vector<MeshPrimitive*> meshPrimitives = meshInstances.at(i)->expandPrimitives();
        objects.insert(objects.end(), meshPrimitives.begin(), meshPrimitives.end());
    }
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
    expandPrimitives();
    computeShadableBoundedHierarchy();
    computeFieldBoundedHierarchy();
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
    expandPrimitives();
    computeShadableBoundedHierarchy();
    computeFieldBoundedHierarchy();
}
