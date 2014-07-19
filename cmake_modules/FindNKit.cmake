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

## - Find NKit
## TODO
## 1) Handling all find_package args(for detail see cmake --help-command find_package)
## 2) Hand pkgconfig
##

##
## nkit_version__ - detect nkit version
## input  : NKIT_INCLUDE_DIR
## TODO: input NKIT_LIBRARY_DIR
## outout : NKIT_VERSION_{MAJOR,MINOR,MICRO,BUILD}, NKIT_VERSION
##
macro(nkit_version__)
  file(READ ${NKIT_INCLUDE_DIR}/nkit/version.h cont__)
  string(REGEX REPLACE
         "^(.*)define NKIT_VERSION_MAJOR \"([0-9]+).*$" "\\2"
         NKIT_VERSION_MAJOR "${cont__}")
  string(REGEX REPLACE
         "^(.*)define NKIT_VERSION_MINOR \"([0-9]+).*$" "\\2"
         NKIT_VERSION_MINOR "${cont__}")
  string(REGEX REPLACE
         "^(.*)define NKIT_VERSION_MICRO \"([0-9]+).*$" "\\2"
         NKIT_VERSION_MICRO "${cont__}")
  string(REGEX REPLACE
    "^(.*)define NKIT_VERSION_BUILD \"([0-9a-zA-Z]+).*$" "\\2"
         NKIT_VERSION_BUILD "${cont__}")
  if ((NKit_FIND_VERSION_MAJOR LESS NKIT_VERSION_MAJOR)
      AND (NKit_FIND_VERSION_MINOR LESS NKIT_VERSION_MINOR)
      AND (NKit_FIND_VERSION_PATCH LESS NKIT_VERSION_MICRO))
    message(SEND_ERROR
      "nkit have wrong version ${NKIT_VERSION} at I${NKIT_INCLUDE_DIR},L${NKIT_LIBRARIES}")
  endif()
  set(NKIT_VERSION
    "${NKIT_VERSION_MAJOR}.${NKIT_VERSION_MINOR}.${NKIT_VERSION_MICRO}.${NKIT_VERSION_BUILD}"
    CACHE INTERNAL "nkit version full")
endmacro(nkit_version__)


## Start here
if ((NKit_FIND_VERSION_MAJOR LESS 0) OR (NKit_FIND_VERSION_MAJOR EQUAL 0)
   AND (NKit_FIND_VERSION_MINOR LESS 0) OR (NKit_FIND_VERSION_MINOR EQUAL 0)
   AND (NKit_FIND_VERSION_PATCH LESS 30) OR (NKit_FIND_VERSION_PATCH EQUAL 0))
   message(SEND_ERROR
           "This module did not optimize for this version greater than 0.0.30.X")
endif()

set(NKit_DIR_PREFIX__    nkit/)
set(NKit_INCLUDE_DIRS__ /usr/local/include /usr/include)
set(NKit_LIB_DIRS__     /usr/local/lib /usr/lib)

if (DEFINED ENV{NKIT_ROOT})
  set(NKIT_ROOT $ENV{NKIT_ROOT})
endif()

if (DEFINED NKIT_ROOT)
  set(NKit_INCLUDE_DIRS__ ${NKIT_ROOT}/include)
  set(NKit_LIB_DIRS__     ${NKIT_ROOT}/lib)
  set(NKit_FIND_OPTS__    NO_CMAKE NO_CMAKE_SYSTEM_PATH)
endif()

find_path(NKIT
          NAMES ${NKit_DIR_PREFIX__}
          HINTS ${NKit_INCLUDE_DIRS__}
          ${NKit_FIND_OPTS__})

if (NOT ${NKIT} STREQUAL "NKIT-NOTFOUND")
  set(NKIT_INCLUDE_DIR ${NKIT} CACHE INTERNAL "nkit headers path")

  ## Find dynamic nkit
  if (DEFINED NKIT_USE_DYN_LIB)
    
    message(SEND_ERROR "NKIT_USE_DYN_LIB not supported yet!")

  ## Find static nkit
  else()
    find_library(NKit_LIBRARY__ NAMES libnkit.a nkit.lib HINTS ${NKit_LIB_DIRS__})
    if (NOT ${NKit_LIBRARY__} STREQUAL "NKIR_LIBRARY-NOTFOUND")
      set(NKIT_LIBRARIES ${NKit_LIBRARY__} CACHE INTERNAL "nkit library")
    else()
      message(SEND_ERROR "Could not find 'libnkit.a' ('${NKit_LIB_DIRS__}')")
    endif()

  endif()
  nkit_version__()
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NKit
      DEFAULT_MSG NKIT_INCLUDE_DIR NKIT_LIBRARIES NKIT_VERSION)
mark_as_advanced(NKIT_INCLUDE_DIR NKIT_LIBRARIES NKIT_VERSION)

