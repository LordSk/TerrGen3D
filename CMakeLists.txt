project(TerrGen3D)
cmake_minimum_required(VERSION 2.8)
file(GLOB CPP_FILES "src/*.cpp")
file(GLOB H_FILES "src/*.h")
set(SRC_FILES ${CPP_FILES} ${H_FILES})
include_directories("src")

# cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_definitions("-std=c++11 -m64 -DBGFX_CONFIG_RENDERER_OPENGL=1")
add_executable(${PROJECT_NAME} ${SRC_FILES})

# find Noise lib
find_package(Noise REQUIRED)
if(${NOISE_FOUND})
    include_directories(${NOISE_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${NOISE_LIBRARY})
endif()

# find bgfx
find_package(bgfx REQUIRED)
if(${BGFX_FOUND})
    include_directories(${BGFX_INCLUDE_DIR})
    include_directories(${BX_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${BGFX_LIBRARY})
endif()

# find GLFW lib
find_package(GLFW REQUIRED)
if(${GLFW_FOUND})
    include_directories(${GLFW_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})
endif()


