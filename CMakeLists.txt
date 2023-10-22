set(CMAKE_PROJECT_VERSION, 0)
set(CMAKE_PROJECT_VERSION_MAJOR, 69)
set(CMAKE_PROJECT_VERSION_MINOR, 42)
set(CMAKE_PROJECT_VERSION_PATCH, 0)

project(Diagonals)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(dependencies)

add_subdirectory(dependencies/glfw-3.3.8)
include_directories(dependencies/glfw-3.3.8/include)

add_compile_definitions(GLEW_STATIC)
add_subdirectory(dependencies/glew-2.1.0)
include_directories(dependencies/glew-2.1.0/include)

add_subdirectory(dependencies/glm)

include_directories(dependencies/imgui)

include_directories(src)
include_directories(src/vendor)

file(GLOB_RECURSE SOURCES
    dependencies/imgui/backends/imgui_impl_glfw.cpp
    dependencies/imgui/backends/imgui_impl_opengl3.cpp
    dependencies/imgui/imgui.cpp
    dependencies/imgui/imgui_draw.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/imgui_widgets.cpp

    dependencies/stb/stb_image.cpp

    src/*.h
    src/*.cpp
)

set(APP_NAME diagonals)

add_executable(${APP_NAME} ${SOURCES})

target_link_libraries(${APP_NAME} GL GLEW glfw ${GLFW_LIBRARIES})