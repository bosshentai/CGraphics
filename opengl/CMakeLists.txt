cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "-fpermissive -Wno-format-zero-length")

add_executable(opengl main.cpp utils/Utils.h utils/pch.cpp utils/pch.h utils/Const.h utils/CommonValues.h stbImage/stb_image.h window/Window.cpp window/Window.h texture/Texture.cpp texture/Texture.h camera/Camera.cpp camera/Camera.h model/Model.cpp model/Model.h mesh/Mesh.cpp mesh/Mesh.h light/DirectionalLight.cpp light/DirectionalLight.h shadow/ShadowMap.cpp shadow/ShadowMap.h light/Light.cpp light/Light.h shadow/OmniShadowMap.cpp shadow/OmniShadowMap.h shader/Shader.cpp shader/Shader.h light/SpotLight.cpp light/SpotLight.h light/PointLight.cpp light/PointLight.h material/Material.cpp material/Material.h)
target_link_libraries(opengl -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lassimp)

