project(FreeHeroes)

cmake_minimum_required(VERSION 2.8)

add_executable(extractor extractor.cpp)
target_link_libraries(extractor engine)

add_executable(82m2wav 82m2wav.cpp)
target_link_libraries(82m2wav engine)
target_link_libraries(82m2wav ${SDL_LIBRARY})

add_executable(til2img til2img.cpp)
target_link_libraries(til2img engine)
target_link_libraries(til2img ${PNG_LIBRARIES})
target_link_libraries(til2img ${SDL_LIBRARY})
target_link_libraries(til2img ${SDL_IMAGE_LIBRARIES})
target_link_libraries(til2img ${SDL_MIXER_LIBRARIES})

add_executable(icn2img icn2img.cpp)
target_link_libraries(icn2img engine)
target_link_libraries(icn2img ${PNG_LIBRARIES})
target_link_libraries(icn2img ${SDL_LIBRARY})
target_link_libraries(icn2img ${SDL_IMAGE_LIBRARIES})
target_link_libraries(icn2img ${SDL_MIXER_LIBRARIES})
target_link_libraries(icn2img ${SDL_TTF_LIBRARIES})

add_executable(xmi2mid xmi2mid.cpp)
target_link_libraries(xmi2mid engine)
target_link_libraries(xmi2mid ${SDL_LIBRARY})
