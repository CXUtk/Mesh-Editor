cmake_minimum_required(VERSION 3.8)

project(glfw LANGUAGES CXX)

set(SOURCES_GLAD "glad.c"
                 "glad.h"
                 "KHR/khrplatform.h")

add_library(glad STATIC ${SOURCES_GLAD})