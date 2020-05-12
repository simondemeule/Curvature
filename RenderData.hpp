//
//  RenderData.hpp
//  Bent
//
//  Created by Simon Demeule on 2019-04-06.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Ray.hpp"
#include "ShadableObject.hpp"
#include "ShadableObjectIntersection.hpp"
#include "BoundedHierarchy.hpp"
#include "MeshData.hpp"
#include "MeshInstance.hpp"
#include "Light.hpp"
#include "Camera.hpp"


// class holding all information necessary to render a scene
class RenderData {
private:
    void loadInputFile();                                           // creates objects and camera from input file
    void loadTest();                                                // creates objects and camera from predefined test setup
    void applyOutputSettingsFromFocalPlane();                       // calculates and sets output size from camera focal plane
    void applyOutputSettingsFromWidth(int outputWidthNew);          // calculates and sets output size from arbitrary width
    void applyOutputSettingsFromHeight(int outputHeightNew);        // calculates and sets output size from arbitrary height
    void applyTileSettings(int tileSizeNew);                        // calculates and sets tile settings from output size and tile size
    void applyAntiAliasingSettings(int antiAliasingPassesRootNew);  // calculates and sets anti aliasing settings from squared number of passes
    void applyThreadSettings(int threadCountNew);                   // sets thread count manually
    void applyThreadSettings();                                     // sets thread count automatically
    void applyRecursionSettings(int recursionLimitNew);             // sets recursion limit for reflections
    void expandPrimitives();                                        // expands mesh instances into mesh primitives
    void computeBoundedHierarchy();                                 // computes acceleration structure
public:
    // these are public but should never be overwritten externally. too lazy for getters and setters.
    
    std::vector<ShadableObject*> objects;
    std::vector<MeshData*> meshDatas;
    std::vector<MeshInstance*> meshInstances;
    std::vector<Light*> lights;
    Camera* camera;
    BoundedHierarchy<ShadableObject, ShadableObjectIntersection>* boundedHierarchy;
    
    int outputWidth;            // output width in pixels
    int outputHeight;           // output height in pixels
    int tileSize;               // tile size in pixels
    int tileCountWidth;         // tile count, horizontally
    int tileCountHeight;        // tile count, vertically
    int tileCount;              // tile count, total
    int antiAliasingPassesRoot; // number of anti aliasing passes square rooted
    int antiAliasingPasses;     // number of anti aliasing passes
    int threadCount;            // number of threads for rendering
    int recursionLimit;         // recursion limit for reflections
    
    std::string outputFileName;     // file path for output image
    std::string inputFileName;      // file path for input scene
    std::string workingDirectory;   // directory where input file resides (necessary for properly loading meshes)
    
    // constructor for tests with fancy features
    RenderData(int outputWidthNew, int tileSizeNew, int antiAliasingPassesRootNew, int threadCountNew, int recursionLimitNew, std::string workingDirectoryNew, std::string outputFilePathNew);
    
    // constructor for assignment files
    RenderData(std::string workingDirectoryNew, std::string inputFileNameNew, std::string outputFileNameNew);
};
