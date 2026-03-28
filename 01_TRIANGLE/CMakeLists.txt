cmake_minimum_required(VERSION 3.14)
project(VulkanTut
    VERSION 1.0.0
    LANGUAGES CXX C
)
cmake_policy(SET CMP0072 NEW)

include(FetchContent)



# fetch GLFW from github mirror
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)

# Tells GLFW not to build the Wayland backend. Without this, CMake tries to find Wayland headers and either fails silently or builds a broken backend.
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)

# Explicitly forces the X11 backend. Without the FORCE, if CMake already cached a wrong value from a previous configure run, this would be ignored — FORCE overwrites it regardless.
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

# Explicitly forces the X11 backend. Without the FORCE, if CMake already cached a wrong value from a previous configure run, this would be ignored — FORCE overwrites it regardless.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)




# make sure vulkan is found and added
find_package(Vulkan REQUIRED)

if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    link_directories(${Vulkan_LIBRARY_DIRS})
endif ()


# create main executable
add_executable(VulkanTut 
    src/vulkantut.cpp 
    src/vulkantut.hpp 
    src/logging.h
)

# link libraries to executable
target_link_libraries(VulkanTut glfw ${Vulkan_LIBRARY})