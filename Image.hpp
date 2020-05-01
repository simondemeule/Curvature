//
//  Image.hpp
//  project
//
//  Created by Simon Demeule on 2019-04-02.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#pragma once

#define cimg_display 0
#include "CImg.h"

#include <glm/glm.hpp>
#include <string>

// image with writing and save to file functionality
class Image {
private:
    cimg_library::CImg<int> image; 
public:
    
    Image(int width, int height);
    
    void write(glm::vec3 color, int x, int y);
    void saveToFile(std::string filePath);
};
