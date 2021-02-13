cmake_minimum_required(VERSION 3.8)

project(glad LANGUAGES CXX)

set(SOURCES_GLAD "glad.c"
                 
                 "KHR/khrplatform.h")

add_library(glad STATIC ${SOURCES_GLAD})