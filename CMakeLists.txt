cmake_minimum_required(VERSION 2.8)

project(student_toolkit)
add_executable(${PROJECT_NAME} "main.cpp" "png_toolkit.cpp" "filters.cpp" "Manager.cpp", "png_toolkit.h"
    "stb_image.h" "stb_image_write.h" "Filters.h" "Libraries.h" "Manager.h", "Definitions.h")
