cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

set(EXTRA_COMPONENT_DIRS App Peripherals User_HAL $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(Riego)
