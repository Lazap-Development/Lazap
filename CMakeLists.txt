cmake_minimum_required(VERSION 4.1)
project(Lazap VERSION 0.9.0)

#-----------------------------------------------------------------------------------------------------------------------
# Options & Configuration
#-----------------------------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Strip debug symbols in release
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependency Options
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(LUNASVG_INSTALL OFF CACHE BOOL "" FORCE)
set(CPR_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries only")
set(CPR_BUILD_TESTS OFF CACHE BOOL "Disable tests")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(B_PRODUCTION_MODE ON CACHE BOOL "Strip all development features such as hot-reload and absolute filepaths" FORCE)
endif()

#-----------------------------------------------------------------------------------------------------------------------
# Dependencies (FetchContent)
#-----------------------------------------------------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.92.5-docking
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG        v5.5.4
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(stb
    URL                  https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    DOWNLOAD_NO_EXTRACT  TRUE
)

FetchContent_Declare(lunasvg
    GIT_REPOSITORY https://github.com/sammycage/lunasvg.git
    GIT_TAG        v3.5.0
)

FetchContent_Declare(tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        e7aaccca3fa3dbde9818ab8313250f3da4976e37
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.12.0
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(battery-embed
    GIT_REPOSITORY https://github.com/batterycenter/embed
    GIT_TAG        465081903d97ff1ed05e1fd5d0b3c8032a4a26a6
)

FetchContent_MakeAvailable(
  imgui glaze stb lunasvg tomlplusplus cpr battery-embed
)

#-----------------------------------------------------------------------------------------------------------------------
# Target Definition
#-----------------------------------------------------------------------------------------------------------------------
string(TOLOWER "${PROJECT_NAME}" EXECUTABLE_NAME)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(${EXECUTABLE_NAME}
    ${SRC_FILES}
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

#-----------------------------------------------------------------------------------------------------------------------
# Resources & Embedding
#-----------------------------------------------------------------------------------------------------------------------
if(WIN32)
  set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/src/resources.rc")
  if(EXISTS ${RESOURCE_FILE})
    target_sources(${EXECUTABLE_NAME} PRIVATE ${RESOURCE_FILE})
  endif()
endif()

file(GLOB_RECURSE ASSET_FILES "${CMAKE_SOURCE_DIR}/assets/*")

foreach(ASSET_FILE ${ASSET_FILES})
  file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}" "${ASSET_FILE}")
  b_embed(${EXECUTABLE_NAME} "${REL_PATH}" ${EXECUTABLE_NAME})
endforeach()

#-----------------------------------------------------------------------------------------------------------------------
# Compile Options & Linking
#-----------------------------------------------------------------------------------------------------------------------

# Get rid of extra terminal window in release build (Windows)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    WIN32_EXECUTABLE $<CONFIG:Release>
)

# Compiler Warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/vendor/glad")
add_library(glad STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}/
    ${stb_SOURCE_DIR}/
    ${tomlplusplus_SOURCE_DIR}/include
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    glfw
    glad
    glaze::glaze
    cpr::cpr
    lunasvg
    $<$<PLATFORM_ID:Linux>:X11>
    $<$<PLATFORM_ID:Windows>:dwmapi>
    $<$<AND:$<CXX_COMPILER_ID:GNU>,$<PLATFORM_ID:Windows>>:opengl32.a>
    $<$<CXX_COMPILER_ID:MSVC>:opengl32.lib>
)

#-----------------------------------------------------------------------------------------------------------------------
# Packaging (CPack)
#-----------------------------------------------------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Lazap Development")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A native-like attempt to unify clients with one fast, modern, and cross-platform solution")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_COMPONENTS_ALL "${PROJECT_NAME}App")

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)

if(ARCH STREQUAL "amd64" OR ARCH STREQUAL "x86_64")
  set(ARCH "x86_64")
elseif(ARCH MATCHES "aarch64|arm64")
  set(ARCH "aarch64")
endif()

set(CPACK_SYSTEM_NAME "${ARCH}")

if(WIN32)
  install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION .
        COMPONENT "${PROJECT_NAME}App"
    )

  set(CPACK_GENERATOR "WIX")
  set(CPACK_WIX_UPGRADE_GUID "9A72C574-EA60-495B-B6AE-64279086CA62")
  set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/assets/icons/lazap/lazap.ico")
  set(CPACK_WIX_LICENSE_FILE "${CMAKE_SOURCE_DIR}/LICENSE.md")
  set(CPACK_WIX_SHORTCUTS "DesktopFolder;Lazap")
  set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

elseif(UNIX AND NOT APPLE)
  set(MAIN_COMPONENT "${PROJECT_NAME}App")

  install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION bin
        COMPONENT ${MAIN_COMPONENT}
    )

  install(FILES ${CMAKE_SOURCE_DIR}/assets/lazap.desktop
        DESTINATION share/applications
        COMPONENT ${MAIN_COMPONENT}
    )

  install(FILES ${CMAKE_SOURCE_DIR}/assets/icons/lazap/lazap.png
        DESTINATION share/icons/hicolor/512x512/apps
        COMPONENT ${MAIN_COMPONENT}
    )

  set(CPACK_INSTALLED_DIRECTORIES "")
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PROJECT_NAME};${MAIN_COMPONENT};/")

  set(CPACK_PACKAGE_ICON "lazap.png")
  set(CPACK_GENERATOR "DEB;RPM;TGZ;AppImage")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Porya Dashtipour <contact@porya.me>")
  set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
endif()

include(CPack)
