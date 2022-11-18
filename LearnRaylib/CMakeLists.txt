cmake_minimum_required(VERSION 3.23)
project(LearnRaylib)

set(CMAKE_CXX_STANDARD 14)

add_executable(LearnRaylib main.cpp include/Camera.h include/CommonValues.h include/DirectionalLight.h include/Light.h include/Material.h include/Mesh.h include/Model.h include/Scene.h include/Shader.h include/ShadowMap.h include/SpotLight.h include/Texture.h)
