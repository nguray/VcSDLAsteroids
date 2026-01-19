cmake_minimum_required(VERSION 3.10)

project(SDLAsteroids VERSION 0.1 LANGUAGES C)

set(PROJECT_SOURCES
	bullet.c
	bullet.h
	explosion.c
	explosion.h
	game.c
	game.h
	rock.c
	rock.h
	ship.c
	ship.h
	main.c
	saucer.c
	saucer.h
)

add_executable(SDLAsteroids ${PROJECT_SOURCES})

set(SDL2_STATIC_LIBRARIES FALSE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(fmt REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

#target_compile_features(SDLAsteroids PUBLIC cxx_std_20)

target_compile_definitions(SDLAsteroids PRIVATE SDL2_SHARED)

if(MINGW)
    message("--> MSYS2 " $ENV{USER})
    add_compile_definitions(_MSYS)
  target_link_libraries(SDLAsteroids mingw32 SDL2main SDL2 SDL2_ttf SDL2_mixer SDL2_image fmt)
endif()

if(LINUX)
    message("--> LINUX " $ENV{USER})
    add_compile_definitions(_LINUX)
    target_link_libraries(SDLAsteroids SDL2main SDL2 SDL2_ttf SDL2_mixer SDL2_image fmt m)
endif()