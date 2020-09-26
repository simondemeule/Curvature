//
//  main.cpp
//  template
//
//  Created by Simon Demeule on 19-01-18.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

// TODO
// front / back intersections are a bit messy and inconsistent. review that.
// split RenderData into RenderCore and RenderSettings
// move AA code from RenderThread to RenderCore
// start implementing BVH tree
// eventually split Mesh into MeshData and MeshInstance, add normal and UV support, refactor OBJlodaer to MeshLoader

#define GLM_ENABLE_EXPERIMENTAL

//#include "Tester.hpp"

#include "RenderData.hpp"
#include "RenderScheduler.hpp"

int main(int argc, const char * argv[]) {
    /*
    Tester tester = Tester();
    tester.doTheTest();
    */
    RenderData renderDataExtended(1000, 32, 2, 16, 2, "","Test.bmp");
    RenderScheduler renderScheduler(&renderDataExtended);
    renderScheduler.renderAndSaveImage();
    /*
    int numSteps = 180;
    for(int i = 0; i < numSteps; i++) {
        std::string path;
        path.append("Frame");
        path.append(std::to_string(i));
        path.append(".bmp");
        RenderData renderData(1920, 32, 2, 16, 2, "", path, glm::radians(360.0 / numSteps * i));
        RenderScheduler renderScheduler(&renderData);
        renderScheduler.renderAndSaveImage();
    }
     */
    return 0;
}
