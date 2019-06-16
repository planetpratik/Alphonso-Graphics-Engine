cmake_minimum_required (VERSION 3.14)

project(AlphonsoEngine)

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

file(GLOB ENGINE_SOURCE_FILES
    "source/*.cpp"
    "source/*.h"
)

if (MSVC)
	# remove default warning level from CMAKE_CXX_FLAGS_INIT
    string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_compile_options(/W4 /WX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

if( CMAKE_BUILD_TYPE STREQUAL "" )
	set( CMAKE_BUILD_TYPE "debug" )
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)
find_package(tinyobjloader REQUIRED)

# Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
endif()

# Include Header files for Vulkan and GLFW3
include_directories(AFTER ${VULKAN_PATH}/Include)
include_directories(${glfw3_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${shaderc_INCLUDE_DIR})
include_directories(${tinyobjloader_INCLUDE_DIR})

add_executable(AlphonsoEngine ${ENGINE_SOURCE_FILES})

target_compile_definitions(AlphonsoEngine PRIVATE VK_USE_PLATFORM_WIN32_KHR)

target_link_libraries(AlphonsoEngine Vulkan::Vulkan tinyobjloader::tinyobjloader glfw opengl32  ${SHADERC_LIB} )