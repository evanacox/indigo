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
#include "klog/print.h"

extern "C" void frt::__frt_assert_fail(const char* condition,
    const char* message,
    frt::CSourceLocation location) noexcept {
  klog::writeln("[libfrt] assertion failed! [at = ", location.file, ':', location.line, " in ", location.function, "]");
  klog::writeln("  condition: `", condition, '`');
  klog::writeln("  message: '", message, '\'');

  indigo::native::halt();
}

extern "C" void frt::__frt_tried_alloc(const char* message, frt::CSourceLocation location) noexcept {
  klog::writeln("[libfrt] tried to allocate! [at = ",
      location.file,
      ':',
      location.line,
      " in ",
      location.function,
      "]");
  klog::writeln("  message: '", message, '\'');

  indigo::native::halt();
}

extern "C" void frt::__frt_bounds_fail(const char* message, frt::CSourceLocation location) noexcept {
  klog::writeln("[libfrt] tried to read out-of-bounds! [at = ",
      location.file,
      ':',
      location.line,
      " in ",
      location.function,
      "]");
  klog::writeln("  message: '", message, '\'');

  indigo::native::halt();
}