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

#include "./print.h"
#include "frt/types/source_location.h"

namespace klog {
  namespace internal {
    class TransparentLoc {
    public:
      // NOLINTNEXTLINE(google-explicit-constructor)
      constexpr TransparentLoc(const char* value, frt::SourceLocation loc = frt::SourceLocation::current())
          : value_{value},
            loc_{frt::move(loc)} {}

      [[nodiscard]] constexpr const char* value() const {
        return value_;
      }

      [[nodiscard]] constexpr frt::SourceLocation loc() {
        return frt::move(loc_);
      }

    private:
      const char* value_;
      frt::SourceLocation loc_;
    };
  } // namespace internal

#ifdef INDIGO_DEBUG
  /// Prints out a debug message with accompanying source info
  ///
  /// \param fake_loc A magical parameter that picks up both a (C-string) message and a source location.
  ///                 Used because parameter packs can't be first, and I need an optional argument.
  /// \param args The arguments to print out in the debug message.
  template <typename... Args> void debug(internal::TransparentLoc fake_loc, Args&&... args) {
    auto loc = fake_loc.loc();

    // [ file.cc:44 in klog::debug] some message! 12345
    klog::writeln("[", loc.file_name(), ":", loc.line(), " in ", loc.function_name(), "] ", fake_loc.value(), args...);
  }
#else
  template <typename... Args> void debug(const Args&... /*unused*/) {
    /* no-op */
  }
#endif
} // namespace klog