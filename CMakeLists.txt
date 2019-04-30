# CMake build file - use CMake 3.2 or later to build WDT and its dependencies.
#
# mkdir build; cd build; cmake .. -DBUILD_TESTING=on; make -j
# omit -DBUILD_TESTING=on if you don't want the extra dependencies for
# testing (but testing is good !)
# (at fb:
# cd local; mkdir wdt_build; cd wdt_build
# cmake31 ~/fbcode/wdt -DFOLLY_SOURCE_DIR=$HOME/fbcode -DBUILD_TESTING=on
# make -j
# )
#
#  Copyright (c) 2014-present, Facebook, Inc.
#  All rights reserved.
#
#  This source code is licensed under the BSD-style license found in the
#  LICENSE file in the root directory of this source tree. An additional grant
#  of patent rights can be found in the PATENTS file in the same directory.
#

cmake_minimum_required(VERSION 3.2)

# There is no C per se in WDT but if you use CXX only here many checks fail
# Version is Major.Minor.YYMMDDX for up to 10 releases per day (X from 0 to 9)
# Minor currently is also the protocol version - has to match with Protocol.cpp
project("WDT" LANGUAGES C CXX VERSION 1.30.1704260)

# On MacOS this requires the latest (master) CMake (and/or CMake 3.1.1/3.2)
# WDT itself works fine with C++11 (gcc 4.8 for instance) but more recent folly
# started to require C++14, so you can either get an older snapshot of folly
# and set "11" below or use this:
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# somehow 'option' for this doesn't seeem to work/I don't know how to make it
set(BUILD_SHARED_LIBS off CACHE Bool "build shared libs")

# CMake default behavior should be to set rpath when needed (non system install)
# it's not so let's set this for now:
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


# Optimized by default
# TODO: This doesn't seem to work / sets default to "" instead of Release...
# set(CMAKE_BUILD_TYPE Release CACHE String "build type")
# So hardcoding for now:
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-msse4.2 -mpclmul -Wno-write-strings")
#set(CMAKE_CXX_FLAGS "-msse4.2 -mpclmul -Wextra -Wsign-compare -Wunused-variable")
#set(CMAKE_CXX_FLAGS "-msse4.2 -mpclmul -Wextra -Wsign-compare -Wunused-variable -Wconversion -Wsign-conversion")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "_bin/wdt")

# Check that we have the Folly source tree
set(FOLLY_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../folly" CACHE path
    "Folly source tree (folly/Conv.h should be reachable from there")
# Check for folly - TODO: this doesn't work well for relative paths
# (because of relative to build dir vs relative to source tree for -I)
if(NOT EXISTS "${FOLLY_SOURCE_DIR}/folly/Conv.h")
  MESSAGE(FATAL_ERROR "${FOLLY_SOURCE_DIR}/folly/Conv.h not found
Fix using:
(in a sister directory of the wdt source tree - same level:)
git clone https://github.com/facebook/folly.git
or change FOLLY_SOURCE_DIR (use ccmake or -DFOLLY_SOURCE_DIR=...)
")
endif()


# The part of folly that isn't pure .h and we use:
set (FOLLY_CPP_SRC
"${FOLLY_SOURCE_DIR}/folly/Conv.cpp"
"${FOLLY_SOURCE_DIR}/folly/Demangle.cpp"
"${FOLLY_SOURCE_DIR}/folly/hash/Checksum.cpp"
"${FOLLY_SOURCE_DIR}/folly/hash/detail/ChecksumDetail.cpp"
"${FOLLY_SOURCE_DIR}/folly/hash/detail/Crc32cDetail.cpp"
"${FOLLY_SOURCE_DIR}/folly/hash/detail/Crc32CombineDetail.cpp"
"${FOLLY_SOURCE_DIR}/folly/lang/ColdClass.cpp"
"${FOLLY_SOURCE_DIR}/folly/ScopeGuard.cpp"
)

# WDT's library proper - comes from: ls -1 *.cpp | grep -iv test
add_library(wdt_min
util/WdtSocket.cpp
util/ClientSocket.cpp
util/EncryptionUtils.cpp
util/DirectorySourceQueue.cpp
ErrorCodes.cpp
util/FileByteSource.cpp
util/FileCreator.cpp
Protocol.cpp
WdtThread.cpp
util/ThreadsController.cpp
ReceiverThread.cpp
Receiver.cpp
WdtTransferRequest.cpp
Reporting.cpp
util/ThreadTransferHistory.cpp
SenderThread.cpp
Sender.cpp
util/ServerSocket.cpp
Throttler.cpp
WdtOptions.cpp
util/FileWriter.cpp
util/TransferLogManager.cpp
util/SerializationUtil.cpp
util/Stats.cpp
WdtBase.cpp
WdtResourceController.cpp
util/CommonImpl.cpp
)
# Source files that depend on gflags and provide flags -> options init
set (WDT_FLAGS_RELATED_SRC
util/WdtFlags.cpp
Wdt.cpp
wdtCmdLine.cpp
ReceiverHandler.cpp
SenderHandler.cpp
)
add_library(wdt ${WDT_FLAGS_RELATED_SRC})

target_link_libraries(wdt wdt_min)



# Folly uses boost system and filesystem
#set(Boost_USE_STATIC_LIBS on)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# We use std:: threads
find_package(Threads) # this will set ${CMAKE_THREAD_LIBS_INIT} (ie pthreads)


# double-conversion
find_path(DOUBLECONV_INCLUDE_DIR double-conversion/double-conversion.h)
find_library(DOUBLECONV_LIBRARY double-conversion)
# Glog
find_path(GLOG_INCLUDE_DIR glog/logging.h)
find_library(GLOG_LIBRARY glog)
# Gflags
find_path(GFLAGS_INCLUDE_DIR gflags/gflags.h)
find_library(GFLAGS_LIBRARY gflags)
# OpenSSL's crypto lib
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# You can also add jemalloc to the list if you have it/want it
target_link_libraries(wdt_min
  folly4wdt
  ${GLOG_LIBRARY}
  ${GFLAGS_LIBRARY}
  ${Boost_LIBRARIES}
  ${DOUBLECONV_LIBRARY}
  ${OPENSSL_CRYPTO_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT} # Must be last to avoid link errors
)

# What we need to build the part of folly we use:

include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckCXXSourceCompiles)
# For WDT itself:
check_function_exists(posix_fallocate HAS_POSIX_FALLOCATE)
check_function_exists(sync_file_range HAS_SYNC_FILE_RANGE)
check_function_exists(posix_memalign HAS_POSIX_MEMALIGN)
check_function_exists(posix_fadvise HAS_POSIX_FADVISE)
# C based check (which fail with the c++ setting thereafter...)
check_function_exists(clock_gettime FOLLY_HAVE_CLOCK_GETTIME)
# was: check_library_exists(rt clock_gettime "" FOLLY_HAVE_CLOCK_GETTIME)
check_function_exists(memrchr FOLLY_HAVE_MEMRCHR)
set(SAVE_CMRL ${CMAKE_REQUIRED_LIBRARIES}) #globals are evil/ugly
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
check_function_exists(pthread_atfork FOLLY_HAVE_PTHREAD_ATFORK)
set(CMAKE_REQUIRED_LIBRARIES ${SAVE_CMRL}) #globals are evil/ugly
# Needed until Cmake issue #15361 is addressed
# All the check_function_exists should be before this line
# or else they will not work'

set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_CXX11_STANDARD_COMPILE_OPTION})
check_include_file_cxx(linux/sockios.h WDT_HAS_SOCKIOS_H)
#check_function_exists(clock_gettime FOLLY_HAVE_CLOCK_GETTIME)
check_cxx_source_compiles("#include <type_traits>
      #if !_LIBCPP_VERSION
      #error No libc++
      #endif
      int main() {return 0;}" FOLLY_USE_LIBCPP)
check_cxx_source_compiles(
"extern \"C\" void cmkcheckweak() __attribute__((weak));
int main(int argc, char** argv) {
  return (cmkcheckweak) ? 1 : 0 ;
}" FOLLY_HAVE_WEAK_SYMBOLS)
# Now record all this :
# Folly's:
configure_file(build/folly-config.h.in folly/folly-config.h)
# Wdt's config/version
configure_file(WdtConfig.h.in wdt/WdtConfig.h)

# Malloc stuff  tied to not supporting weaksympbols
if (NOT FOLLY_HAVE_WEAK_SYMBOLS)
  list(APPEND FOLLY_CPP_SRC "${FOLLY_SOURCE_DIR}/folly/memory/detail/MallocImpl.cpp")
  message(STATUS "no weak symbols, adding MallocImpl to folly src")
endif()

add_library(folly4wdt  ${FOLLY_CPP_SRC})
target_link_libraries(folly4wdt ${GLOG_LIBRARY} ${DOUBLECONV_LIBRARY})

# Order is important - inside fb we want the above
# folly-config.h to be picked up instead of the fbcode one
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FOLLY_SOURCE_DIR})
include_directories(${DOUBLECONV_INCLUDE_DIR})
include_directories(${GLOG_INCLUDE_DIR})
include_directories(${GFLAGS_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

#add_executable(wdtbin main.cpp ${WDT_FLAGS_RELATED_SRC})
#set_target_properties(wdtbin PROPERTIES COMPILE_DEFINITIONS "STANDALONE_APP")

#target_link_libraries(wdtbin wdt_min)

### Install rules
#set_target_properties(wdtbin PROPERTIES RUNTIME_OUTPUT_NAME "wdt")
#install(TARGETS wdtbin wdt wdt_min folly4wdt
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib
#  )



add_custom_target(combined ALL
  #COMMAND ar x $<TARGET_FILE:folly4wdt>
  COMMAND ar x $<TARGET_FILE:wdt>
  COMMAND ar x $<TARGET_FILE:wdt_min>
  COMMAND ar rc libwdtcombined.a *.o
  COMMAND rm -f *.o)



### Install header files

# Find the . files in the root directory

file(GLOB headers "*.h")
foreach(header ${headers})
  install(FILES ${header} DESTINATION include/wdt)
endforeach()

# Install the .h files in the util with directory structure maintained

install(DIRECTORY "${PROJECT_SOURCE_DIR}/util" DESTINATION include/wdt
        FILES_MATCHING PATTERN "*.h")

# wcp script
install(PROGRAMS test/wcp.sh DESTINATION bin RENAME wcp)

