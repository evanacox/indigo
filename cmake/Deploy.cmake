# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

function(indigo_copy_image)
    message(STATUS "Adding post-build target to copy ${CMAKE_BINARY_DIR}/kernel8.img to ${INDIGO_RPI_SD_CARD}/kernel8.img")
    add_custom_command(TARGET indigo_img POST_BUILD
            COMMAND test ! -e ${INDIGO_RPI_SD_CARD} || ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/kernel8.img" "${INDIGO_RPI_SD_CARD}/kernel8.img"
            COMMENT "Copying ${CMAKE_BINARY_DIR}/kernel8.img to ${INDIGO_RPI_SD_CARD}/kernel8.img if ${INDIGO_RPI_SD_CARD} exists...")
endfunction()