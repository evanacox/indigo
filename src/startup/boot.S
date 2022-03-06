//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.      //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

.global _start
.global __indigo_halt

.section ".text.boot"

// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location
_start:
    mrs     x5, mpidr_el1
    and     x5, x5, #3
    cbz     x5, .setup_stack
    bl      __indigo_halt
.setup_stack:
    ldr     x5, =__stack_start
    mov     sp, x5           // set stack pointer properly
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
.clear_loop:                 // need to clear out BSS or our data will be screwed up
    cbz     w6, .enter_cxx
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, .clear_loop
.enter_cxx:
    bl      __indigo_entry  // jump to C++ code, shouldn't return. if it does, just loop infinitely
    bl      __indigo_halt

