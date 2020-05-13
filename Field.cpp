//
//  Field.cpp
//  Bent
//
//  Created by Simon Demeule on 2019-10-01.
//  Copyright © 2019 Simon Demeule. All rights reserved.
//

#include "Field.hpp"

#include <glm/glm.hpp>

FieldIntersection Field::intersection(Ray ray) {
    BoundedObjectIntersection intersectionBox = boundingBox.intersection(ray);
    FieldIntersection intersectionField;
    intersectionField.exists = intersectionBox.exists;
    if(intersectionBox.exists) {
        intersectionField.distance = intersectionBox.distance;
        intersectionField.origin = ray.origin + ray.direction * intersectionBox.distance;
        intersectionField.incident = ray.direction;
    }
    return intersectionField;
}
