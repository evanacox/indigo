//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "kio/sleep.h"

void kio::sleep(int cycles) {
  asm volatile(".loop: \n"
               "        subs %[count], %[count], #1\n"
               "        nop\n"
               "        bne .loop"
               : "=r"(cycles)
               : [count] "0"(cycles)
               : "cc");
}