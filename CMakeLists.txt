cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/CMakeModules"
  GIT_TAG v0.12.4
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  ULF_MDU_EIN
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX)

add_library(ULF_MDU_EIN INTERFACE ${SRC})
add_library(ULF::MDU_EIN ALIAS ULF_MDU_EIN)

target_compile_features(ULF_MDU_EIN INTERFACE cxx_std_23)

# https://github.com/espressif/esp-idf/issues/17773
if(PROJECT_IS_TOP_LEVEL AND NOT ESP_PLATFORM)
  target_include_directories(ULF_MDU_EIN INTERFACE include)
  target_common_warnings(ULF_MDU_EIN INTERFACE)
  target_common_errors(ULF_MDU_EIN INTERFACE)
else()
  target_include_directories(ULF_MDU_EIN SYSTEM INTERFACE include)
endif()

if(NOT TARGET MDU::MDU)
  cpmaddpackage("gh:ZIMO-Elektronik/MDU@0.19.5")
endif()

target_link_libraries(ULF_MDU_EIN INTERFACE MDU::MDU)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  add_subdirectory(examples)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC include/*.*pp tests/*.*pp)
  add_clang_format_target(ULF_MDU_EINFormat OPTIONS -i FILES ${SRC})
  add_include_what_you_must_target(ULF_MDU_EINIncludeWhatYouMust TARGET
                                   ULF_MDU_EIN)
endif()

if(BUILD_TESTING
   AND PROJECT_IS_TOP_LEVEL
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(tests)
endif()
