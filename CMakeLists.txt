cmake_minimum_required(VERSION 3.20)
project(r-type_server LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.x)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    src/main.cpp

    # src/Music.cpp
    # src/Texture2D.cpp
    # src/AudioDevice.cpp
    src/Scene.cpp
    src/SceneManager.cpp
    src/Entity.cpp
    src/Window.cpp
    src/Core.cpp

    # src/Text.cpp

    # src/ButtonCallbacks.cpp
    # src/MouseCallbacks.cpp
    # src/GamepadStickCallbacks.cpp
    # src/Sound.cpp

    # src/generateMap.cpp

    # src/systems/AudioSystem.cpp
    src/systems/GraphicSystem.cpp
    src/systems/NewGameSystem.cpp

    # src/systems/EventSystem.cpp
    # src/systems/CollideSystem.cpp
    # src/systems/ParticlesSystem.cpp
    # src/systems/AISystem.cpp
    src/components/Component.cpp

    # src/components/EventListener.cpp
    # src/components/Player.cpp
    # src/components/Position.cpp
    # src/components/AVector.cpp
    # src/components/Velocity.cpp
    # src/components/HitboxComponent.cpp
    # src/components/ParticleCloud.cpp
    # src/components/Bomb.cpp
    # src/components/UIComponent.cpp
    # src/components/AIPlayer.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC

    # src/components
    src/exceptions
    src/systems
    src
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND_EXPAND_LISTS)
endif()

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    target_compile_options(${PROJECT_NAME}
        PRIVATE

        # -Wall

        # -Wextra
    )
endif(CMAKE_COMPILER_IS_GNUCXX)

install(TARGETS ${PROJECT_NAME})
cmake_minimum_required(VERSION 3.16)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../"
)