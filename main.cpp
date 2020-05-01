//
//  main.cpp
//  template
//
//  Created by Simon Demeule on 19-01-18.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "RenderData.hpp"
#include "RenderScheduler.hpp"

int main(int argc, const char * argv[]) {
    // constructor for basic scene file.
    // thread count is auto detected. meshes are assumed to be in the working directory.
    //
    // renderData("working_directory/", "input_file_name.txt", "output_file_name.bmp")
    RenderData renderData("scenes/", "scene5.txt", "mesh_scene5_.bmp");
    
    // constructor for hardcoded test scene with extended features. I know! No extra credit. But I wanted to do it anyways, so I might as well show it!
    // it has antialiasing and recursive reflections. the "extendedFeatures.bmp" render took 98645 milliseconds to complete on a 6 core intel i9-8950HK
    //
    // renderData(image_width, tile_size, antialiasing_passes_square_rooted, thread_count, recursive_reflection_depth_limit, "working_directory", "output_file_name.bmp")
    RenderData renderDataExtended(2000, 64, 4, 12, 2, "","extendedFeatures.bmp");
    
    // constructor for renderer
    RenderScheduler renderScheduler(&renderData);
    // method for rendering
    renderScheduler.renderAndSaveImage();
    return 0;
}
