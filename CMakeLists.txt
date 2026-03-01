cmake_minimum_required(VERSION 3.10)
project(Tiled VERSION 1.0)

add_executable(${PROJECT_NAME} 
    Tiled/src/main.cpp
    Tiled/ico/app.rc
)

# SOURCES FILES
set(SOURCES
    Tiled/src/Window.cpp
    Tiled/src/DebugLog.cpp
    Tiled/src/Textures.cpp
    Tiled/src/Shader.cpp
    Tiled/src/Theme.cpp
    Tiled/src/Cursor.cpp
    Tiled/src/Time.cpp
    Tiled/src/Camera.cpp
    Tiled/src/ElementGUI/ElementGUI.cpp
    Tiled/src/Dialogs/Dialog.cpp
    Tiled/src/Dialogs/Palette.cpp
    Tiled/src/Tileset.cpp
    Tiled/src/Map.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

source_group(TREE ${CMAKE_SOURCE_DIR}/Tiled/src
             PREFIX "Source Files"
             FILES ${SOURCES})

# HEADERS FILES
set(HEADERS
    Tiled/include/Window.hpp
    Tiled/include/DebugLog.hpp
    Tiled/include/Textures.hpp
    Tiled/include/Shader.hpp
    Tiled/include/Theme.hpp
    Tiled/include/Cursor.hpp
    Tiled/include/Time.hpp
    Tiled/include/Camera.hpp
    Tiled/include/ElementGUI/ElementGUI.hpp
    Tiled/include/Dialogs/Dialog.hpp
    Tiled/include/Dialogs/Palette.hpp
    Tiled/include/Tileset.hpp
    Tiled/include/Map.hpp
    )

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})

source_group(TREE ${CMAKE_SOURCE_DIR}/Tiled/include
             PREFIX "Header Files"
             FILES ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Tiled/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# set the SFML lib
set(SFML_DIR "C:/SFML-3.0.2/lib/cmake/SFML")

# set the packages SFML
find_package(SFML 3.0.2 COMPONENTS Graphics Window System Audio REQUIRED)
target_link_libraries(${PROJECT_NAME} 
	SFML::Graphics
        SFML::Window
        SFML::System
	SFML::Audio
)

# set the working directory (assets)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Tiled"
    VS_DEBUGGER_WORKING_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Tiled"
)

# set the path for SFML bin
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;C:\\SFML-3.0.2\\bin"
)