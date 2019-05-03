## set the project name to help import libs
project(GameClient)
set(GAMEDATA_FOLDER "data/client")

## add the files to be compiled here
set(SOURCE_FILES
        "../src/client/Source/main.cpp"
        "../src/client/Source/game.cpp"
        ../src/client/Source/Network/ClientConnection.cpp
        ../src/client/Source/Scenes/SceneManager.cpp
        ../src/client/Source/Scenes/MenuScene.cpp
        ../src/client/Source/Scenes/TitleScene.cpp
        ../src/client/Source/AnimatedSprite.cpp
        ../src/client/Source/Scenes/GameScene.cpp
        ../src/client/Source/Tile.cpp 
        ../src/client/Source/Map.cpp
        ../src/client/Source/UI/ClickArea.cpp
        ../src/client/Source/UI/UI.cpp
        ../src/client/Source/UI/Button.cpp
        ../src/client/Source/UI/TextBox.cpp
        ../src/client/Source/UI/ProgressBar.cpp
        ../src/client/Source/Locator.cpp
        ../src/client/Source/UI/TextInput.cpp
        ../src/client/Source/UI/ChatWindow.cpp
        ../src/client/Source/InputManager.cpp
        ../src/client/Source/Characters/ClientScout.cpp
        ../src/client/Source/UI/UIManager.cpp 
        ../src/client/Source/UI/InfoDisplay.cpp
        ../src/client/Source/Scenes/CharacterSelect.cpp
        ../src/client/Source/Network/NetworkManager.cpp
        ../src/client/Source/Characters/Character.cpp
        ../src/client/Source/Characters/CharacterManager.cpp
        ../src/client/Source/Characters/ClientBuilder.cpp
        ../src/client/Source/Characters/ClientMedic.cpp
        ../src/client/Source/Characters/ClientSoldier.cpp
        ../src/client/Source/ClientEvent.cpp
        ../src/client/Headers/ClientEvent.h
        ../src/client/Source/UI/Image.cpp
        ../src/client/Source/AudioManager.cpp
        ../src/client/Source/FileReader.cpp 
        ../src/client/Source/UI/CharInfoDisplay.cpp
        ../src/client/Source/UI/OptionsMenu.cpp ../src/client/Source/UI/StockpileDisplay.cpp)

set(HEADER_FILES
        "../src/client/Headers/game.h"
        ../src/client/Headers/Network/ClientConnection.h
        ../src/client/Headers/Scenes/Scene.h
        ../src/client/Headers/Scenes/SceneManager.h
        ../src/client/Headers/Scenes/TitleScene.h
        ../src/client/Headers/Scenes/MenuScene.h
        ../src/client/Headers/AnimatedSprite.h
        ../src/client/Headers/Scenes/GameScene.h
        ../src/client/Headers/Tile.h 
        ../src/client/Headers/Map.h
        ../src/client/Headers/UI/Button.h
        ../src/client/Headers/UI/ClickArea.h
        ../src/client/Headers/UI/UI.h
        ../src/client/Headers/UI/TextBox.h
        ../src/client/Headers/UI/ProgressBar.h
        ../src/client/Headers/Locator.h
        ../src/client/Headers/UI/TextInput.h
        ../src/client/Headers/InputManager.h
        ../src/client/Headers/Characters/ClientScout.h
        ../src/client/Headers/UI/UIManager.h 
        ../src/client/Headers/UI/InfoDisplay.h
        ../src/client/Headers/UI/ChatWindow.h
        ../src/client/Headers/Scenes/CharacterSelect.h
        ../src/client/Headers/Network/NetworkManager.h
        ../src/client/Headers/Scenes/PersistentClientData.h
        ../src/client/Headers/GameRenderer.h 
        ../src/client/Source/GameRenderer.cpp
        ../src/client/Headers/Characters/Character.h
        ../src/client/Headers/Characters/CharacterManager.h
        ../src/client/Headers/Characters/ClientBuilder.h
        ../src/client/Headers/Characters/ClientMedic.h
        ../src/client/Headers/Characters/ClientSoldier.h
        ../src/client/Source/ClientEvent.cpp
        ../src/client/Headers/ClientEvent.h
        ../src/client/Headers/UI/Image.h
        ../src/client/Headers/AudioManager.h
        ../src/client/Headers/UI/OptionsMenu.h
        ../src/client/Headers/FileReader.h 
        ../src/client/Headers/UI/CharInfoDisplay.h ../src/client/Headers/UI/StockpileDisplay.h)

## the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

## the client is dependent on the game and network library
add_dependencies(${PROJECT_NAME} GameLib enetpp)
target_link_libraries(${PROJECT_NAME} GameLib enetpp)

## these are the build directories
get_target_property(CLIENT ${PROJECT_NAME} NAME)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${CLIENT}/bin")

SET(ENABLE_SOUND ON CACHE BOOL "Adds SoLoud to the Project" FORCE)
target_link_libraries(${PROJECT_NAME} GameLib soloud)

## important build scripts
include(build/compilation)
include(libs/soloud)
include(tools/itch.io)
include(libs/asge)

## hide console unless debug build ##
if (NOT CMAKE_BUILD_TYPE STREQUAL  "Debug" AND WIN32)
    target_compile_options(${PROJECT_NAME} -mwindows)
endif()


