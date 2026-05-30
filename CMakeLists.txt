cmake_minimum_required(VERSION 4.2)
cmake_policy(SET CMP0167 OLD)

project(Trabalho01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(main
        main.cpp
        external/glad/src/gl.c
        utils/utils.h
        utils/utils.cpp
        utils/convexHull.h
        utils/convexHull.cpp
)

target_include_directories(main PRIVATE
        external/glfw/include
        external/glad/include
)

target_link_directories(main PRIVATE
        external/glfw/lib-mingw-w64
)

target_link_libraries(main
        opengl32
        glfw3
)

add_executable(viewConvexHull
        viewConvexHull.cpp
        external/glad/src/gl.c
        utils/utils.h
        utils/utils.cpp
        utils/convexHull.h
        utils/convexHull.cpp
)

target_include_directories(viewConvexHull PRIVATE
        external/glfw/include
        external/glad/include
)

target_link_directories(viewConvexHull PRIVATE
        external/glfw/lib-mingw-w64
)

target_link_libraries(viewConvexHull
        opengl32
        glfw3
)

add_executable(theme
        theme.cpp
        external/glad/src/gl.c
        utils/utils.h
        utils/utils.cpp
        utils/convexHull.h
        utils/convexHull.cpp
)

target_include_directories(theme PRIVATE
        external/glfw/include
        external/glad/include
)

target_link_directories(theme PRIVATE
        external/glfw/lib-mingw-w64
)

target_link_libraries(theme
        opengl32
        glfw3
)

add_executable(main3d
        main3d.cpp
        external/glad/src/gl.c
        utils/utils.h
        utils/utils.cpp
        utils/convexHull.h
        utils/convexHull.cpp
)

target_include_directories(main3d PRIVATE
        external/glfw/include
        external/glad/include
)

target_link_directories(main3d PRIVATE
        external/glfw/lib-mingw-w64
)

target_link_libraries(main3d
        opengl32
        glfw3
)

add_executable(theme3d
        theme3d.cpp
        external/glad/src/gl.c
        utils/utils.h
        utils/utils.cpp
        utils/convexHull.h
        utils/convexHull.cpp
)

target_include_directories(theme3d PRIVATE
        external/glfw/include
        external/glad/include
)

target_link_directories(theme3d PRIVATE
        external/glfw/lib-mingw-w64
)

target_link_libraries(theme3d
        opengl32
        glfw3
)
