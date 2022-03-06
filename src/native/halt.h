//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#pragma once

extern "C" {
  [[noreturn]] void __indigo_halt();
}

namespace indigo::native {
  [[noreturn]] inline void halt() {
    __indigo_halt();
  }
} // namespace indigo::native