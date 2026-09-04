cmake_minimum_required(VERSION 3.16)

project(Face_Tracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenCV_DIR "/opt/homebrew/Cellar/opencv@4/4.14.0_6/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

add_executable(Face_Tracker main.cpp calibration.cpp)

target_link_libraries(Face_Tracker PRIVATE ${OpenCV_LIBS})

set(HAARCASCADE_PATH "/opt/homebrew/Cellar/opencv@4/4.14.0_6/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")
configure_file(${HAARCASCADE_PATH} ${CMAKE_BINARY_DIR}/haarcascade_frontalface_default.xml COPYONLY)
target_compile_definitions(Face_Tracker PRIVATE HAARCASCADE_PATH="${HAARCASCADE_PATH}")