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
.global __indigo_entry
.global __indigo_halt

.section ".text.boot"

// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location
_start:
    mrs     x5, mpidr_el1       // code starts executing on all cores, for now, we put all of them
    and     x5, x5, #0xFF       // except the primary one in a halt loop
    cbz     x5, .setup_env
    bl      __indigo_halt
.setup_env:
    ldr     x5, =_start         // set stack pointer properly
    mov     sp, x5
    ldr     x5, =__bss_start    // zero out .bss section
    ldr     w6, =__bss_size     // make loop counter start at `__bss_size`
    cbz     w6, .enter_cxx      // exit if loop counter == 0
.bss_loop:
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, .bss_loop       // loop again if non-zero
.enter_cxx:
    bl      __indigo_entry      // jump to C++ code, shouldn't return.
    b       __indigo_halt       // if it does, just halt

