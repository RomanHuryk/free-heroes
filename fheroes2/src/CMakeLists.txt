project(FreeHeroes)

cmake_minimum_required(VERSION 2.8)

option(ENABLE_ZLIB "Enable zlib (comressed save files)" ON)
option(ENABLE_MIXER "Enable SDL_mixer" OFF)
option(ENABLE_AUDIOCD "Enable audio cd support" OFF)
option(ENABLE_UNICODE "Enable unicode (translation and ttf font)" OFF)
option(ENABLE_IMAGE "Enable SDL_image (cache image, icn2png)" OFF)
option(ENABLE_XML "Enable tinyxml, used for load alt. resources" OFF)
option(ENABLE_NETWORK "Enable network" OFF)
option(ENABLE_AI "Enable simple AI (Non-commercial License)" OFF)
option(ENABLE_GITHASH "Enable git HEAD hash in version number" ON)
option(ENABLE_TOOLS "Enable building tools" ON)

if(ENABLE_IMAGE)
	set(ENABLE_ZLIB ON)
endif()

# also can be set:
# system locale dir
#add_definitions(-DCONFIGURE_FHEROES2_LOCALEDIR=/path/to/locale/dir)
# system fheroes2 game dir
#add_definitions(-DCONFIGURE_FHEROES2_DATA=/path/to/data/dir) 

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -pedantic -DWITH_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DBUILD_RELEASE")

# TODO: vs and mingw support

find_package(SDL REQUIRED)
include_directories("${SDL_INCLUDE_DIR}")

if(ENABLE_ZLIB)
	add_definitions(-DWITH_ZLIB)
	find_package(ZLIB REQUIRED)
	include_directories("${ZLIB_INCLUDE_DIR}")
endif()

if(ENABLE_MIXER)
	add_definitions(-DWITH_MIXER)
	find_package(SDL_mixer REQUIRED)
	include_directories("${SDL_MIXER_INCLUDE_DIR}")
endif()

if(ENABLE_AUDIOCD)
	add_definitions(-DWITH_AUDIOCD)
endif()

if(ENABLE_UNICODE)
	add_definitions(-DWITH_TTF)
	find_package(SDL_ttf REQUIRED)
	include_directories("${SDL_TTF_INCLUDE_DIR}")
endif()

if(ENABLE_IMAGE)
	add_definitions(-DWITH_IMAGE)
	find_package(PNG REQUIRED)
	find_package(SDL_image REQUIRED)
	include_directories("${SDL_IMAGE_INCLUDE_DIR}")
endif()

if(ENABLE_XML)
	add_definitions(-DWITH_XML)
	include_directories(xmlccwrap)
	add_subdirectory(xmlccwrap)
endif()

if(ENABLE_NETWORK)
	add_definitions(-DWITH_NET_DISABLE)
	find_package(SDL_net REQUIRED)
	include_directories("${SDL_NET_INCLUDE_DIR}")
endif()

if(ENABLE_GITHASH)
	execute_process(COMMAND git rev-parse --short HEAD
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	add_definitions(-DGIT_HASH="${GIT_HASH}")
endif()

add_subdirectory(engine)
include_directories(engine)
include_directories(fheroes2)

file(GLOB SOURCES_LIST "fheroes2/agg/*.*"
	"fheroes2/ai/*.*"
	"fheroes2/army/*.*"
	"fheroes2/battle/*.*"
	"fheroes2/castle/*.*"
	"fheroes2/dialog/*.*"
	"fheroes2/game/*.*"
	"fheroes2/gui/*.*"
	"fheroes2/heroes/*.*"
	"fheroes2/image/*.*"
	"fheroes2/kingdom/*.*"
	"fheroes2/maps/*.*"
	"fheroes2/monster/*.*"
	"fheroes2/objects/*.*"
	"fheroes2/pocketpc/*.*"
	"fheroes2/resource/*.*"
	"fheroes2/spell/*.*"
	"fheroes2/system/*.*"
	"fheroes2/test/*.*")

if(ENABLE_AI)
	file(GLOB AI_SOURCES_LIST "fheroes2/ai/simple/*.*")
else()
	file(GLOB AI_SOURCES_LIST "fheroes2/ai/empty/*.*")
endif()

add_executable(FreeHeroes 
	${SOURCES_LIST}
	${AI_SOURCES_LIST}
)

target_link_libraries(FreeHeroes engine)
if(ENABLE_XML)
	target_link_libraries(xmlccwrap)
endif()

if(ENABLE_TOOLS)
	add_subdirectory(tools)
endif()