cmake_minimum_required(VERSION 3.10)

project(SoftwareRender VERSION 1.0)

add_library(SoftwareRender Win32PixelWindow.cpp)
add_definitions(-DUNICODE)

target_include_directories(SoftwareRender
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})