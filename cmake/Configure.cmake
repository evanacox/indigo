# ======---------------------------------------------------------------====== #
#                                                                             #
#  Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.       #
#                                                                             #
#  Use of this source code is governed by a BSD-style license that can be     #
#  found in the LICENSE.txt file at the root of this project, or at the       #
#  following link: https://opensource.org/licenses/BSD-3-Clause               #
#                                                                             #
# ======---------------------------------------------------------------====== #

list(APPEND INDIGO_BARE
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -nostartfiles
        -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=indigo
        -mgeneral-regs-only)

list(APPEND INDIGO_WARNINGS
        -Wall
        -Wextra
        -Wcast-qual
        -Wconversion-null
        -Wmissing-declarations
        -Woverlength-strings
        -Wpointer-arith
        -Wunused-local-typedefs
        -Wunused-result
        -Wvarargs
        -Wvla
        -Wwrite-strings
        -Werror
        -ftemplate-backtrace-limit=20
        -fconcepts-diagnostics-depth=5)

function(indigo_configure_target TARGET)
    target_compile_options(${TARGET} PUBLIC ${INDIGO_WARNINGS} ${INDIGO_BARE})
    target_compile_features(${TARGET} PUBLIC cxx_std_20)
    target_link_options(${TARGET} PUBLIC -nostartfiles)
    target_link_libraries(${TARGET} PUBLIC frt etl)

    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        target_compile_definitions(${TARGET} PUBLIC INDIGO_DEBUG)
    endif ()

    if (${INDIGO_RPI_3})
        target_compile_definitions(${TARGET} PUBLIC INDIGO_RPI_3)
    else ()
        target_compile_definitions(${TARGET} PUBLIC INDIGO_RPI_4)
    endif ()
endfunction()

function(indigo_configure_library TARGET)
    target_compile_options(${TARGET} PUBLIC ${INDIGO_BARE})
endfunction()