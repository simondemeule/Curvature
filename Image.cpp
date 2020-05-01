//
//  Image.cpp
//  project
//
//  Created by Simon Demeule on 2019-04-02.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Image.hpp"

#include <iostream>

Image::Image(int width, int height) : image(width, height, 1, 3, 0) {}

int floatTo8Bit(double from) {
    int temp = (int) 255.0 * from;
    return (temp < 0) ? 0 : (temp > 255 ? 255 : temp);
}

void Image::write(glm::vec3 color, int x, int y) {
    image(x, y, 0) = floatTo8Bit(color.r);
    image(x, y, 1) = floatTo8Bit(color.g);
    image(x, y, 2) = floatTo8Bit(color.b);
}

void Image::saveToFile(std::string filePath) {
    std::cout << "Writing Image to File" << std::endl;
    image.save(filePath.c_str());
    std::cout << "Done" << std::endl;
}
