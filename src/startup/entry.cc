//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2022 Evan Cox <evanacox00@gmail.com>. All rights reserved.      //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "entry.h"
#include "../native/halt.h"
#include "kio/sleep.h"
#include "kio/uart.h"
#include <cstdint>

extern volatile std::uint8_t* __bss_start; // NOLINT(readability-identifier-naming)
extern volatile std::uint8_t* __bss_end;   // NOLINT(readability-identifier-naming)

// dtb is a 32-bit pointer
extern "C" void __indigo_entry(std::uint64_t /*dtb*/) {
  kio::uart_init();

  while (true) {
    kio::uart_writeln("Hello, World!");
  }

  indigo::native::halt();
}
