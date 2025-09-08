cmake_minimum_required(VERSION 3.24)
project(RendererVulkanPS1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan SDK
find_package(Vulkan REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)  

# GLM
add_subdirectory(lib/glm)    

# Executable
add_executable(RendererVulkanPS1 src/main.cpp)

target_link_libraries(RendererVulkanPS1 PRIVATE
  Vulkan::Vulkan
  glfw
  glm::glm
)

# Build type specific toggles
target_compile_definitions(RendererVulkanPS1 PRIVATE
  $<$<CONFIG:Debug>:ENABLE_VALIDATION_LAYERS=1;LOG_VERBOSE=1>
  $<$<CONFIG:Release>:NDEBUG>
)

if (MSVC)
  target_compile_options(RendererVulkanPS1 PRIVATE 
    /permissive-
    $<$<CONFIG:Debug>:/W4 /Zi /Od>      # more warnings, no optimizations
    $<$<CONFIG:Release>:/W3 /O2>          # soft warning, optimization
  )

  set_property(TARGET RendererVulkanPS1 PROPERTY 
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" 
  )
endif()
