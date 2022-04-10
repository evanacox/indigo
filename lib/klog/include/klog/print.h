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

#include "etl/log.h"
#include "etl/optional.h"
#include "etl/string.h"
#include "etl/to_string.h"
#include "frt/core/limits.h"
#include "frt/types/concepts.h"
#include "kio/uart.h"

namespace klog {
  namespace internal {
    template <frt::Integral T> constexpr auto max_digits_base(std::size_t base) {
      // ex: digits10 == ⌊Limits<T>::digits * log10(2)⌋
      // ex: digits8 == ⌊Limits<T>::digits * log8(2)⌋
      switch (base) {
        case 10:
          // already have a constant for this
          return frt::NumericLimits<T>::digits10;
        case 16:
          // log16(2) = 0.25, approx by / 4
          return frt::NumericLimits<T>::digits / 4;
        case 8:
          // log8(2) = 0.33 repeating, approx by / 3
          return frt::NumericLimits<T>::digits / 3;
        default: return 32; // ??
      }
    }

    template <frt::Integral T, std::size_t Base> using StringForBase = etl::string<internal::max_digits_base<T>(Base)>;

    template <frt::Integral T> StringForBase<T, 10> klog_into_string(T value) {
      StringForBase<T, 10> result;

      etl::to_string(value, result);

      return result;
    }

    template <frt::Integral T, std::size_t Base> [[nodiscard]] constexpr etl::format_spec padding(char c, int width) {
      etl::format_spec spec;

      spec.fill(c).width((width == -1) ? internal::max_digits_base<T>(Base) : width);

      return spec;
    }

    template <typename T> void uart_write(const T& first) {
      // intentional ADL here
      auto data = klog_into_string(first);

      kio::uart_write_raw(data.c_str());
    }

    inline void uart_write(const char* first) {
      kio::uart_write_raw(first);
    }

    inline void uart_write(char first) {
      if (first == '\n') {
        kio::uart_write_raw('\r');
      }

      kio::uart_write_raw(first);
    }

    template <typename T, typename... Rest> void uart_write(const T& first, const Rest&... rest) {
      internal::uart_write(first);
      internal::uart_write(rest...);
    }
  } // namespace internal

  class ZeroPad {
  public:
    constexpr explicit ZeroPad(int width) : width_{width} {}

    template <typename T, std::size_t Base> [[nodiscard]] constexpr etl::format_spec format() const {
      return internal::padding<T, Base>('0', width_);
    }

  private:
    int width_;
  };

  class Pad {
  public:
    constexpr explicit Pad(char c, int width) : pad_c_{c}, width_{width} {}

    constexpr explicit Pad(char c) : pad_c_{c} {}

    template <typename T, std::size_t Base> [[nodiscard]] constexpr etl::format_spec format() const {
      return internal::padding<T, Base>(pad_c_, width_);
    }

  private:
    char pad_c_;
    int width_ = -1;
  };

  /// Helper type for printing out a number in hex format.
  ///
  /// Takes an arbitrary integer and some optional formatting information,
  /// and gives an object that can be printed out. When printed out, it displays
  /// a number in hex format.
  template <frt::Integral T> class Hex {
  public:
    Hex() = delete;

    constexpr explicit Hex(T value) : Hex(value, etl::format_spec{}) {}

    constexpr explicit Hex(T value, etl::format_spec spec) : spec_{spec}, value_{value} {
      spec_.hex();
    }

    constexpr explicit Hex(T value, ZeroPad width) : Hex(value, width.format<T, 16>()) {}

    constexpr explicit Hex(T value, Pad pad) : Hex(value, pad.format<T, 16>()) {}

    friend internal::StringForBase<T, 16> klog_into_string(const Hex<T>& value) {
      internal::StringForBase<T, 16> result;

      etl::to_string(value.value_, result, value.spec_);

      return result;
    }

  private:
    etl::format_spec spec_;
    T value_;
  };

  /// Writes output to the UART without a newline following it.
  ///
  /// \param args Arguments to print out directly to the UART.
  template <typename... Args> void write(const Args&... args) {
    internal::uart_write(args...);
  }

  /// Writes output to the UART with a newline following it.
  ///
  /// \param args Arguments to print out directly to the UART.
  template <typename... Args> void writeln(const Args&... args) {
    internal::uart_write(args..., '\n');
  }
} // namespace klog