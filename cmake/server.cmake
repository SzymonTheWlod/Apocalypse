## set the project name to help import libs
project(GameServer)
set(GAMEDATA_FOLDER "data/server")

## add the files to be compiled here
set(SOURCE_FILES
        "../src/server/Source/main.cpp"
        ../src/server/Source/Characters/Character.cpp
        ../src/server/Source/Network/ServerConnection.cpp
        ../src/server/Source/ServerTile.cpp
        ../src/server/Source/Scenes/Lobby.cpp
        ../src/server/Source/Locator.cpp
        ../src/server/Source/Scenes/MainGame.cpp
        ../src/server/Source/Map.cpp
        ../src/server/Source/Scenes/ServerCharacterSelect.cpp
        ../src/server/Source/Network/NetworkManager.cpp
        ../src/server/Source/Scenes/PersistentServerData.cpp 
        ../src/server/Source/Characters/ServerScout.cpp 
        ../src/server/Source/Characters/CharacterManager.cpp 
        ../src/server/Source/Characters/ServerMedic.cpp 
        ../src/server/Source/Characters/ServerSoldier.cpp 
        ../src/server/Source/Characters/ServerBuilder.cpp
        ../src/server/Source/EventManager.cpp)

set(HEADER_FILES
        "../src/server/Headers/Network/ServerConnection.h"
        ../src/server/Headers/Characters/Character.h
        ../src/server/Headers/ServerTile.h
        ../src/server/Headers/Scenes/Lobby.h
        ../src/server/Headers/Locator.h
        ../src/server/Headers/Scenes/MainGame.h
        ../src/server/Headers/Map.h
        ../src/server/Headers/Scenes/ServerScene.h
        ../src/server/Headers/Scenes/ServerCharacterSelect.h
        ../src/server/Headers/Network/NetworkManager.h
        ../src/server/Headers/Scenes/PersistentServerData.h
        ../src/server/Headers/Scenes/ClientData.h 
        ../src/server/Headers/Characters/ServerScout.h 
        ../src/server/Headers/Characters/CharacterManager.h 
        ../src/server/Headers/Characters/ServerMedic.h 
        ../src/server/Headers/Characters/ServerSoldier.h 
        ../src/server/Headers/Characters/ServerBuilder.h
        ../src/server/Headers/EventManager.h)

## the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

## the server is dependent on the game and network library
add_dependencies(${PROJECT_NAME} GameLib enetpp)
target_link_libraries(${PROJECT_NAME} GameLib enetpp)

## these are the build directories
get_target_property(SERVER GameServer NAME)
set_target_properties(GameServer
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${SERVER}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${SERVER}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${SERVER}/bin")

file(COPY "${CMAKE_SOURCE_DIR}/data/server" DESTINATION "${CMAKE_BINARY_DIR}/build/${SERVER}/bin")

## important build scripts
include(build/compilation)
include(tools/itch.io)