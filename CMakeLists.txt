cmake_minimum_required(VERSION 3.16)

project(r-type VERSION 1.0.0 LANGUAGES CXX)

set(CLIENT_NAME "r-type_client")
set(SERVER_NAME "r-type_server")
set(LIBRARY_NAME "r-type_library")


# Force Release build by default if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Force MSVC to use Release Qt libraries even in debug mode
if(MSVC)
    add_compile_definitions($<$<CONFIG:Debug>:QT_NO_DEBUG>)
    add_compile_definitions($<$<CONFIG:Debug>:QT_RELEASE>)
endif()

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

set(CMAKE_AUTOMOC_PATH_PREFIX ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

find_package(Qt5 COMPONENTS Core Network Widgets CONFIG REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${LIBRARY_NAME}
    src/ModelAnimation.cpp
    src/Music.cpp
    src/Texture2D.cpp
    src/Camera2D.cpp
    src/Camera3D.cpp
    src/AudioDevice.cpp
    src/Core.cpp
    src/Scene.cpp
    src/SceneManager.cpp
    src/Entity.cpp
    src/Window.cpp
    src/Shape3D.cpp
    src/Model.cpp
    src/Text.cpp
    src/ButtonCallbacks.cpp
    src/MouseCallbacks.cpp
    src/GamepadStickCallbacks.cpp
    src/Sound.cpp
    src/UdpSocket.cpp
    src/Message.cpp
    src/MapGeneration.cpp

    src/systems/AudioSystem.cpp
    src/systems/GraphicSystem.cpp
    src/systems/GameSystem.cpp
    src/systems/EventSystem.cpp
    src/systems/CollideSystem.cpp
    src/systems/ParticlesSystem.cpp
    src/systems/AISystem.cpp
    src/systems/ANetworkSystem.cpp
    src/systems/NetworkServerSystem.cpp
    src/systems/NetworkClientSystem.cpp

    src/components/Component.cpp
    src/components/EventListener.cpp
    src/components/Player.cpp
    src/components/Position.cpp
    src/components/AVector.cpp
    src/components/Velocity.cpp
    src/components/HitboxComponent.cpp
    src/components/Camera2DComponent.cpp
    src/components/Camera3DComponent.cpp
    src/components/Sphere.cpp
    src/components/Cube.cpp
    src/components/Grid.cpp
    src/components/ParticleCloud.cpp
    src/components/Bomb.cpp
    src/components/UIComponent.cpp
    src/components/AIPlayer.cpp
    src/components/Trajectory.cpp
    src/components/Animation2D.cpp
    src/components/Wall.cpp
    src/components/Missile.cpp
    src/components/Enemy.cpp
    src/components/Boss.cpp

    test_functions/TestAnimations2D.cpp
    test_functions/TestCamera2D.cpp
    test_functions/TestCamera3D.cpp
    test_functions/TestTextures.cpp
    test_functions/TestCollisions.cpp
    test_functions/TestModelsCollision.cpp
    test_functions/TestMusic.cpp
    test_functions/TestAudioDevice.cpp
    test_functions/TestSound.cpp
    test_functions/TestModelAnimation.cpp
)
target_include_directories(${LIBRARY_NAME}
    PUBLIC
    src/components
    src/exceptions
    src/systems
    src
)
target_compile_definitions(${LIBRARY_NAME} PUBLIC)
target_link_libraries(${LIBRARY_NAME} raylib; Qt5::Core; Qt5::Network)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${LIBRARY_NAME} -lstdc++fs)
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CLIENT_NAME} src/main_client.cpp)
target_link_libraries(${CLIENT_NAME} ${LIBRARY_NAME})

add_executable(${SERVER_NAME} src/main_server.cpp)
target_link_libraries(${SERVER_NAME} ${LIBRARY_NAME})

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    target_compile_options(${CLIENT_NAME} PRIVATE -Wall -Wextra)
    target_compile_options(${SERVER_NAME} PRIVATE -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCXX)

set_target_properties(
    ${CLIENT_NAME} ${SERVER_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../"
)
