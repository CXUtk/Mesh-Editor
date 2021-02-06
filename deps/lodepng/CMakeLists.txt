cmake_minimum_required(VERSION 3.8)

project(lodepng LANGUAGES CXX)

set(SOURCES_LODE_PNG "lodepng.cpp")

add_library(lodepng STATIC ${SOURCES_LODE_PNG})