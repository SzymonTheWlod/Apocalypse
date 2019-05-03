project(GameLib)

## shared library for the game
add_library(GameLib STATIC
        ../src/gamelib/Source/gamelib.cpp
        ../src/gamelib/Headers/NetworkPacket.h
        ../src/gamelib/Source/Printer.cpp
        ../src/gamelib/Headers/Printer.h
        ../src/gamelib/Source/NetworkPacket.cpp
        ../src/gamelib/Headers/Observer.h
        ../src/gamelib/Headers/Constants.h
        ../src/gamelib/Headers/Subject.h
        ../src/gamelib/Source/Subject.cpp
        ../src/gamelib/Headers/Point.h
        ../src/gamelib/Headers/Resources.h
        ../src/gamelib/Source/Point.cpp ../src/gamelib/Source/Resources.cpp)

## compile language settings
target_compile_features(GameLib PUBLIC cxx_std_17)

add_dependencies(${PROJECT_NAME} enetpp)
target_link_libraries(${PROJECT_NAME} enetpp)

## these are the build directories
get_target_property(GAMELIB GameLib NAME)
set_target_properties(GameLib
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${GAMELIB}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${GAMELIB}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${GAMELIB}/bin")

## include directories for this library
target_include_directories(GameLib
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/external/nlohmann")

include(build/compilation)
include(build/flags)