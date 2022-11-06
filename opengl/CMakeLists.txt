cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 20)

add_executable(opengl main.cpp utils/Utils.h utils/pch.cpp utils/pch.h utils/Const.h utils/CommonValues.h stbImage/stb_image.h window/Window.cpp window/Window.h texture/Texture.cpp texture/Texture.h)
target_link_libraries(opengl -lglut -lglfw -lGLEW -lGL -IGLU )
