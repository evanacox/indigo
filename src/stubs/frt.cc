//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "../native/halt.h"
#include "frt/runtime/assert.h"
#include "frt/runtime/failures.h"
#include "kio/uart.h"

extern "C" void frt::__frt_assert_fail(const char* condition,
    const char* message,
    frt::CSourceLocation location) noexcept {
  kio::uart_writeln("indigo: assertion failed! [at = ", location.file, ':', location.function);
  kio::uart_writeln("  condition: `", condition, '`');
  kio::uart_writeln("  message: '", message, '\'');

  indigo::native::halt();
}

extern "C" void frt::__frt_tried_alloc(const char* message, frt::CSourceLocation location) noexcept {
  kio::uart_writeln("indigo: tried to allocate! [at = ", location.file, ':', location.function);
  kio::uart_writeln("  message: '", message, '\'');

  indigo::native::halt();
}

extern "C" void frt::__frt_bounds_fail(const char* message, frt::CSourceLocation location) noexcept {
  kio::uart_writeln("indigo: tried to access out-of-bounds! [at = ", location.file, ':', location.function);
  kio::uart_writeln("  message: '", message, '\'');

  indigo::native::halt();
}