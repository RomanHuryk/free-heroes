project(FreeHeroes)

cmake_minimum_required(VERSION 2.8)

file(GLOB SOURCES *.*)

add_library(engine ${SOURCES})

target_link_libraries(engine ${SDL_LIBRARY})

if(ENABLE_ZLIB)
	target_link_libraries(engine ${ZLIB_LIBRARIES})
endif()

if(ENABLE_MIXER)
	target_link_libraries(engine ${SDL_MIXER_LIBRARIES})
endif()

if(ENABLE_UNICODE)
	target_link_libraries(engine ${SDL_TTF_LIBRARIES})
endif()

if(ENABLE_IMAGE)
	target_link_libraries(engine ${PNG_LIBRARIES})
	target_link_libraries(engine ${SDL_IMAGE_LIBRARIES})
endif()

if(ENABLE_NETWORK)
	target_link_libraries(engine ${SDL_NET_LIBRARIES})
endif()

if(ENABLE_GITHASH)
	execute_process(COMMAND git rev-parse --short HEAD
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	add_definitions(-DGIT_HASH="${GIT_HASH}")
endif()