cmake_minimum_required(VERSION 3.16)

project(r-type_client VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME})

# Call the cmake that checks and pulls the raylib
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} raylib)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} -lstdc++fs)
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_STANDARD 17)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp
    src/ModelAnimation.cpp
    src/Music.cpp
    src/Texture2D.cpp
    src/Camera.cpp
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

    src/systems/AudioSystem.cpp
    src/systems/GraphicSystem.cpp
    src/systems/GameSystem.cpp
    src/systems/EventSystem.cpp
    src/systems/CollideSystem.cpp
    src/systems/ParticlesSystem.cpp
    src/systems/AISystem.cpp

    src/components/Component.cpp
    src/components/EventListener.cpp
    src/components/Player.cpp
    src/components/Position.cpp
    src/components/AVector.cpp
    src/components/Velocity.cpp
    src/components/HitboxComponent.cpp
    src/components/CameraComponent.cpp
    src/components/Sphere.cpp
    src/components/Cube.cpp
    src/components/Grid.cpp
    src/components/ParticleCloud.cpp
    src/components/Bomb.cpp
    src/components/UIComponent.cpp
    src/components/AIPlayer.cpp

    test_functions/TestAnimations2D.cpp
    test_functions/TestCamera.cpp
    test_functions/TestTextures.cpp
    test_functions/TestCollisions.cpp
    test_functions/TestModelsCollision.cpp
    test_functions/TestMusic.cpp
    test_functions/TestAudioDevice.cpp
    test_functions/TestSound.cpp
    test_functions/TestModelAnimation.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    src/components
    src/exceptions
    src/systems
    src
)

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
        -Wall
        -Wextra
    )
endif(CMAKE_COMPILER_IS_GNUCXX)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../"
)