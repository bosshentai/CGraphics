#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

class Utils
{
  static float degreesToRadians(int deg){return deg *(M_PI / 180.0f);}
  static std::string ReadFile(const char *path);
};


#endif // !Utils_h