cmake_minimum_required(VERSION 3.5)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

project(graphics)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp triangle_app.cpp data_app.cpp uniform_app.cpp file_reader.h file_reader.cpp)

find_package(OpenGL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLFW_INCLUDE_DIRS} /home/jordan/src/sb7code/include)

add_executable(graphics ${SOURCE_FILES})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
target_link_libraries(graphics ${OPENGL_LIBRARIES} /home/jordan/src/sb7code/build/lib/libsb7.a glfw3 X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL rt dl pthread)
