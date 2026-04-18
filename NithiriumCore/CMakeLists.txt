cmake_minimum_required(VERSION 3.20)
project(NithiriumCore LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Add BGFX from vendor ---
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/bgfx)

# --- Find JNI ---
find_package(JNI REQUIRED)
if (NOT JNI_FOUND)
    message(FATAL_ERROR "Could not find JNI.")
endif()

# --- NithiriumCore Shared Library ---
add_library(NithiriumCore SHARED
    src/main/cpp/nithirium_core.cpp
    src/main/cpp/jni/com_nithirium_agent_NithiriumBridge.cpp
)

# --- Include Directories ---
target_include_directories(NithiriumCore PUBLIC
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/public
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/bx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/bimg/include
)

# --- Link Libraries ---
target_link_libraries(NithiriumCore PUBLIC
    bgfx
    bimg
    bx
)

# --- OS Specific Settings ---
if(WIN32)
    target_link_libraries(NithiriumCore PUBLIC Gdi32 psapi user32 kernel32)
    target_compile_definitions(NithiriumCore PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(NithiriumCore PUBLIC "GL" "X11" "pthread")
endif()

# --- Installation ---
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist)
install(TARGETS NithiriumCore
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
