cmake_minimum_required(VERSION 3.24)
project(RendererVulkanPS1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan SDK
find_package(Vulkan REQUIRED)

# GLFW (niente esempi/test/docs)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# GLM
add_subdirectory(lib/glm)

file(GLOB_RECURSE CPP_SOURCE_FILES CONFIGURE_DEPENDS "src/*.hpp" "src/*.cpp")

# Executable
add_executable(RendererVulkanPS1
  ${CPP_SOURCE_FILES}
)

target_include_directories(RendererVulkanPS1 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(RendererVulkanPS1 PRIVATE
  Vulkan::Vulkan
  glfw
  glm::glm
)

if (MSVC)
  target_compile_options(RendererVulkanPS1 PRIVATE
    /permissive-
    $<$<CONFIG:Debug>:/W4 /Zi /Od>
    $<$<CONFIG:Release>:/W3 /O2>
  )
endif()
