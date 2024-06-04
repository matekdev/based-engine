cmake_minimum_required(VERSION 3.28)

project(based-engine LANGUAGES CXX)

# source files
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.hpp")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

# use CPM
include(cmake/CPM.cmake)

# GLFW & IMGUI
CPMAddPackage(
  NAME imgui_cmake_wrapper
  GITHUB_REPOSITORY matekdev/imgui_cmake_wrapper
  GIT_TAG main
)

# GLOG
CPMAddPackage(
  NAME glog
  GITHUB_REPOSITORY google/glog
  GIT_TAG v0.5.0
)
option(BUILD_TESTS "Build tests" OFF)

# ENTT
CPMAddPackage(
  NAME EnTT
  VERSION 3.13.2
  GITHUB_REPOSITORY skypjack/entt
  DOWNLOAD_ONLY True
)

if(EnTT_ADDED)
  add_library(EnTT INTERFACE)
  target_include_directories(EnTT SYSTEM INTERFACE ${EnTT_SOURCE_DIR}/src)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC src)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui_cmake_wrapper glog EnTT)

# Copy shader files
add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders
        ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/shaders
        COMMENT "Copying shaders into binary directory")

# Copy model/texture files
add_custom_target(copy_models ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/models
        ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/models
        COMMENT "Copying models/textures into binary directory")

# Copy font files
add_custom_target(copy_font ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/font
        ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/font
        COMMENT "Copying fonts into binary directory")

add_dependencies(${PROJECT_NAME} copy_shaders copy_models copy_font)