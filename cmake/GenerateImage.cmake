# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

function(indigo_handle_linker_script TARGET LINKER_SCRIPT)
    message(STATUS "Adding dependency on ${LINKER_SCRIPT} to ${TARGET}")
    execute_process(COMMAND dirname ${CMAKE_BINARY_DIR}/${LINKER_SCRIPT} OUTPUT_VARIABLE FOLDER)

    if (NOT TARGET indigo_linker_script)
        add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/${LINKER_SCRIPT}"
                MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT}
                COMMAND mkdir -p ${FOLDER}
                COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT} "${CMAKE_BINARY_DIR}/${LINKER_SCRIPT}")
        add_custom_target(indigo_linker_script ALL
                DEPENDS ${CMAKE_BINARY_DIR}/${LINKER_SCRIPT})
    endif ()

    add_dependencies(${TARGET} indigo_linker_script)
    set_target_properties(${TARGET} PROPERTIES LINK_DEPENDS ${CMAKE_BINARY_DIR}/${LINKER_SCRIPT})
    target_link_options(${TARGET} PUBLIC -T ${CMAKE_BINARY_DIR}/${LINKER_SCRIPT})
endfunction()

function(indigo_create_image TARGET)
    message(STATUS "Adding custom target ${TARGET}.img")

    add_custom_command(OUTPUT kernel8.img
            DEPENDS ${TARGET}
            COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET} kernel8.img)
    add_custom_target(${TARGET}_img ALL
            DEPENDS ${TARGET} kernel8.img)
endfunction()