cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set (PROJECT "opencv_practice")
project (${PROJECT})

find_package (Threads)
find_package (OpenCV REQUIRED)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

set(source_files
    main.cc)

# add_library(${PROJECT}_lib)
add_executable(${PROJECT} ${source_files})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

# Generate extra compilation info used by YCM.
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

target_link_libraries(${PROJECT}
                      ${OpenCV_LIBS})
#                       ${PROJECT}_lib)
