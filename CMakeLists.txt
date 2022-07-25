cmake_minimum_required(VERSION 3.21)
project(app)

set (CMAKE_CXX_STANDARD 17)




include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_TAG         release-2.0.20
    GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)

# set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2-static)


add_executable(app demo/main.cpp)
target_link_libraries(app PRIVATE SDL2main SDL2-static)
# target_include_directories(app PRIVATE include)


if (WIN32)
target_link_options(app PRIVATE -mwindows)
endif (WIN32)