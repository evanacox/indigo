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

// dtb is a 32-bit pointer
extern "C" void __indigo_entry(std::uint64_t /*unused*/) {
  kio::uart_init();
  kio::uart_writeln("Hello, World!");

  indigo::native::halt();
}
