################################################################################
#
#   Copyright 2016 by Norbert Klose (norbert.klose@web.de)
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
#
################################################################################

cmake_minimum_required( VERSION 2.6 )

project( lldb-mi )

set( PACKAGE "libcommon" )
set( PACKAGE_VERSION "0.2")

set( CMAKE_CXX_STANDARD "11")
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS "-g" )
set( CMAKE_CXX_FLAGS_DEBUG "-O0" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set( CMAKE_BUILD_WITH_INSTALL_RPATH True )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH True )
set_property( GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS On )

find_path( LLDB_API_INCLUDE_DIR lldb/API/SBDebugger.h PATHS "${LLDB_API_INCLUDE_DIR}" "${LLDB_PATH}/include" )
if( NOT LLDB_API_INCLUDE_DIR )
    message( FATAL_ERROR "Error: lldb/API/SBDebugger.h not found. Please, set either LLDB_PATH or LLDB_API_INCLUDE_DIR" )
else( NOT LLDB_API_INCLUDE_DIR )
    message( STATUS "Found LLDB C++ API: ${LLDB_API_INCLUDE_DIR}" )
endif( NOT LLDB_API_INCLUDE_DIR )

find_library( LLDB_LIBRARY lldb PATHS "${LLDB_LIBRARY_DIR}" "${LLDB_PATH}/lib" )
if( NOT LLDB_LIBRARY )
    message( FATAL_ERROR "Error: lldb library not found. Please, set either LLDB_PATH or LLDB_LIBRARY" )
else( NOT LLDB_LIBRARY )
    message( STATUS "Found LLDB library: ${LLDB_LIBRARY}" )
endif( NOT LLDB_LIBRARY )

include_directories(
    ${CMAKE_HOME_DIRECTORY}/include
    ${LLDB_API_INCLUDE_DIR}
)

add_library( lldb-mi-interpreter
    src/Command.cpp
    src/Interpreter.cpp
)

target_link_libraries( lldb-mi-interpreter ${LLDB_LIBRARY} )
add_executable( lldb-mi src/lldb-mi.cpp )
target_link_libraries( lldb-mi lldb-mi-interpreter )

install( TARGETS   lldb-mi-interpreter ARCHIVE DESTINATION lib64   )
install( DIRECTORY include/                    DESTINATION include )

export( TARGETS lldb-mi-interpreter FILE lldb-mi-interpreter.cmake )

set( CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "lldb-mi command line interface by Norbert Klose (norbert.klose@web.de)" )
set( CPACK_GENERATOR TGZ )
set( CPACK_SOURCE_PACKAGE_FILE_NAME "lldb-mi-${PACKAGE_VERSION}-src" )
set( CPACK_SOURCE_IGNORE_FILES "build-*;.git*" )
set( CPACK_SOURCE_GENERATOR TGZ )
include( CPack )
