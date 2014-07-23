#   Copyright 2014 Vasiliy Soshnikov (dedok.mad@gmail.com)
#                  Boris T. Darchiev (boris.darchiev@gmail.com)
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

## TODO
## 1) Get revision information from git to ${NKIT_VERSION_MICRO} and ${NKIT_VERSION_BUILD} variables 
## 2) pkgconfig generation
## 3) RPM/DEP packages generation
## 4) CLANG/VS 2008 compiler/linker flags
##

cmake_minimum_required(VERSION 2.8)

project(nkit)

### setup envt
set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake_modules"
    ${CMAKE_MODULE_PATH}
    CACHE INTERNAL "nkit extend module path")

include(SetupCompilerOptions)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                      --force-new-ctest-process --output-on-failure
                      --build-config "$<CONFIGURATION>")
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                      --force-new-ctest-process --output-on-failure)
endif()

include(CTest)
enable_testing()

### entry

find_package(Yajl 2.0.4 REQUIRED)

if (DEFINED ENV{BOOST_VERSION})
    set(BOOST_VERSION $ENV{BOOST_VERSION})
else()
    set(BOOST_VERSION 1.53)
endif()

if (DEFINED ENV{USE_BOOST}
  OR DEFINED USE_BOOST
  OR NOT DEFINED HAVE_STD_CXX_11)
    set(Boost_USE_STATIC_LIBS        ON)
    set(Boost_USE_MULTITHREADED      ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)
    find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS system)
    set(HAVE_BOOST 1 CACHE INTERNAL "boost found")
    set(USE_BOOST 1 CACHE INTERNAL "use boost in nkit backend")
endif()

set(NKIT_VERSION_MAJOR 0)
set(NKIT_VERSION_MINOR 9)
set(NKIT_VERSION_MICRO 0)
set(NKIT_VERSION_BUILD 0)
set(NKIT_VERSION "${NKIT_VERSION_MAJOR}.${NKIT_VERSION_MINOR}.${NKIT_VERSION_MICRO}.${NKIT_VERSION_BUILD}")

# if under Mercurial controll, then get revision info from .hg
set(DOT_HG "${CMAKE_CURRENT_SOURCE_DIR}/.hg")
if (EXISTS ${DOT_HG} AND IS_DIRECTORY ${DOT_HG})
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/Mercurial.cmake)
    if (MERCURIAL_FOUND)
        Mercurial_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_NAME})
        set(NKIT_VERSION_MICRO ${nkit_WC_REVISION})
        set(NKIT_VERSION_BUILD ${nkit_WC_CHANGESET})
        set(NKIT_VERSION "${NKIT_VERSION_MAJOR}"
                         ".${NKIT_VERSION_MINOR}"
                         ".${nkit_WC_REVISION}"
                         ".${nkit_WC_CHANGESET}")
        string(REPLACE ";" "" NKIT_VERSION ${NKIT_VERSION})
    endif()
endif()

if (UNIX OR APPLE)
    set(EXTRA_SYS_LIB
      ${EXTRA_SYS_LIB}
      ${YAJL_LIBRARIES}
      ${Boost_LIBRARIES}
      pthread)
    if (NOT APPLE)
        set(EXTRA_SYS_LIB ${EXTRA_SYS_LIB} rt)
    endif()
else()
    set(EXTRA_SYS_LIB ${YAJL_LIBRARIES} ${Boost_LIBRARIES} "")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
  ${YAJL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/perf)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
