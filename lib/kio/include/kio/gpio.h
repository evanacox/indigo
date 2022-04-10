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

#include "kio/mmio.h"
#include <cstddef>
#include <cstdint>

namespace kio {
  // See pg. 67 of BCM2711 (5.2)
  inline constexpr std::uintptr_t gpio_base = kio::mmio_base + 0x200000;
  inline constexpr std::uintptr_t gpio_function_select0 = kio::gpio_base + 0x00;
  inline constexpr std::uintptr_t gpio_function_select1 = kio::gpio_base + 0x04;
  inline constexpr std::uintptr_t gpio_function_select2 = kio::gpio_base + 0x08;
  inline constexpr std::uintptr_t gpio_function_select3 = kio::gpio_base + 0x0C;
  inline constexpr std::uintptr_t gpio_function_select4 = kio::gpio_base + 0x10;
  inline constexpr std::uintptr_t gpio_function_select5 = kio::gpio_base + 0x14;
  inline constexpr std::uintptr_t gpio_set0 = kio::gpio_base + 0x1C;
  inline constexpr std::uintptr_t gpio_set1 = kio::gpio_base + 0x20;
  inline constexpr std::uintptr_t gpio_clear0 = kio::gpio_base + 0x28;
  inline constexpr std::uintptr_t gpio_clear1 = kio::gpio_base + 0x2C;
  inline constexpr std::uintptr_t gpio_pull_up_down0 = kio::gpio_base + 0xE4;
  inline constexpr std::uintptr_t gpio_pull_up_down1 = kio::gpio_base + 0xE8;
  inline constexpr std::uintptr_t gpio_pull_up_down2 = kio::gpio_base + 0xEC;
  inline constexpr std::uintptr_t gpio_pull_up_down3 = kio::gpio_base + 0xF0;
  inline constexpr int gpio_max_pin = 57;

  /// Defines the "GPIO Function" of a specific pin. Each pin can be in one of 6 alternate states,
  /// or can be an input/output pin.
  enum class GPIOFunction : std::uint32_t {
    input = 0b000,
    output = 0b001,
    alternate_0 = 0b100,
    alternate_1 = 0b101,
    alternate_2 = 0b110,
    alternate_3 = 0b111,
    alternate_4 = 0b011,
    alternate_5 = 0b010,
  };

  /// Sets the GPIO Function of a given pin
  ///
  /// \param pin The pin to set (must be in the range [0, 58])
  /// \param function
  void gpio_function(int pin, GPIOFunction function);

  /// Sets a GPIO pin to its ON state.
  ///
  /// \param pin The pin to update
  void gpio_set(int pin);

  /// Sets a GPIO pin to its OFF state.
  ///
  /// \param pin The pin to update
  void gpio_clear(int pin);

  /// Sets a GPIO pin to `state` (where `true` = set and `false` = clear).
  ///
  /// \param pin The pin to update
  /// \param state The new state for the pin
  void gpio_update(int pin, bool state);

  enum class PullState : std::uint32_t {
    none = 0b00,

#ifdef INDIGO_RPI_4
    up = 0b01,
    down = 0b10,
#else
    up = 0b10,
    down = 0b01;
#endif
  };

  /// Sets the pull state for the specified pin
  void gpio_pull(int pin, PullState state);
} // namespace kio