project(FreeHeroes)

cmake_minimum_required(VERSION 2.8)

file(GLOB SOURCES *.*)

add_library(xmlccwrap ${SOURCES} ${HEADERS})