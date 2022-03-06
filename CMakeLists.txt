# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

cmake_minimum_required(VERSION 3.20)
project(indigo LANGUAGES CXX ASM)

include(cmake/Configure.cmake)
include(cmake/GenerateImage.cmake)

option(INDIGO_RPI_4 "Whether or not to build for RPi-4 (builds for RPi-3 otherwise)" ON)

# "freestanding"
set(FRT_HAS_STDLIB ON)
set(FRT_HAS_COMPARE ON)
set(FRT_HAS_NEW ON)
set(FRT_HAS_MEMCPY ON)
set(FRT_HAS_ASSERT_FAIL ON)
set(FRT_HAS_TRIED_THROW ON)
set(FRT_HAS_BOUNDS_FAIL ON)

add_subdirectory(vendor/freestanding-rt)
add_subdirectory(lib)
add_subdirectory(src)