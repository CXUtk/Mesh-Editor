cmake_minimum_required(VERSION 3.8)

project(imgui LANGUAGES CXX)

aux_source_directory(. IMGUI_SOURCE)

add_library(imgui STATIC ${IMGUI_SOURCE})