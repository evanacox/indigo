/*======---------------------------------------------------------------======*/
/*                                                                           */
/* Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.      */
/*                                                                           */
/* Use of this source code is governed by a BSD-style license that can be    */
/* found in the LICENSE.txt file at the root of this project, or at the      */
/* following link: https://opensource.org/licenses/BSD-3-Clause              */
/*                                                                           */
/*======---------------------------------------------------------------======*/

ENTRY(_start)

SECTIONS
{
    . = 0x80000; /* rpi bootloader jumps to 0x80000 */
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    . = ALIGN(8); /* 8-byte align BSS so we can zero it with `str` */
    .bss (NOLOAD) :
    {
        __bss_start = .;
        *(.bss .bss.*)
        __bss_end = .;
        __bss_size = (__bss_end - __bss_start);
    }
    . = ALIGN(4096);
    _end = .;
    . = _end + 0x400000; /* 4MB stack */
    __stack_start = .;
    /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}