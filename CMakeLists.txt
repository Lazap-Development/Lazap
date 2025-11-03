cmake_minimum_required(VERSION 4.0)
project(Lazap)

#-----------------------------------------------------------------------------------------------------------------------
# Download dependencies for GitHub
include(FetchContent)

FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG glew-cmake-2.2.0
        GIT_SHALLOW TRUE
)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.92.4-docking
        GIT_SHALLOW TRUE
)

FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG v5.5.4  
        GIT_SHALLOW TRUE
)
FetchContent_Declare(
        stb
        URL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
        DOWNLOAD_NO_EXTRACT TRUE
        GIT_SHALLOW TRUE
)


FetchContent_MakeAvailable(glew glfw glaze imgui stb)

# FetchContent_Declare() normally supports EXCLUDE_FROM_ALL but apparently there is a bug
# See https://gitlab.kitware.com/cmake/cmake/-/issues/20167
# As a workaround, we exclude the directories manually
set_property(DIRECTORY ${glew_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)
set_property(DIRECTORY ${glfw_SOURCE_DIR} PROPERTY EXCLUDE_FROM_ALL YES)

#-----------------------------------------------------------------------------------------------------------------------
# Configure C++ version

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-----------------------------------------------------------------------------------------------------------------------
# generate the compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
)
add_executable(${EXECUTABLE_NAME}
        ${SRC_FILES} 

        ${imgui_SOURCE_DIR}/imgui.cpp
        # ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp

        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        )

# Get rid of extra terminal window in release build
set_target_properties(${EXECUTABLE_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

# Add options for warning
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${EXECUTABLE_NAME}
            PRIVATE
            -Wall -Wextra -pedantic
            )
endif()

# Add a define to use the static version of Glew
target_compile_definitions(${EXECUTABLE_NAME}
        PRIVATE
        GLEW_STATIC
        )

# Set the include path
target_include_directories(${EXECUTABLE_NAME}
        PRIVATE
        ${imgui_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/include)

target_include_directories(${EXECUTABLE_NAME}
        PRIVATE
        ${stb_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/include)

# Link against the appropriate libraries
target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
        glfw
        glaze::glaze
        libglew_static
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<PLATFORM_ID:Windows>>:opengl32.a>
        $<$<CXX_COMPILER_ID:MSVC>:opengl32.lib>
        )

# Add assets folder to the build
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/src/assets
)
