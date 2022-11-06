//
// Created by bosshentai on 11/6/22.
//

#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H


#include <cmath>
#include <string>

class Utils {
public:
    static float degreesToRadians(int deg) {return deg * (M_PI / 180.0f);}
    static std::string ReadFile(const char *path);

};


#endif //OPENGL_UTILS_H
