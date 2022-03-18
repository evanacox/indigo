//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "kio/gpio.h"
#include "frt/collections/array.h"
#include "frt/runtime/assert.h"
#include "kio/mmio.h"
#include "kio/sleep.h"
#include <type_traits>

inline constexpr int gpio_register_width = 32;

// number of pins per set/clear register
inline constexpr int pins_per_set_clear = gpio_register_width / 1; // 32, each bit => 1 pin

// the number of pins per function select register
inline constexpr int pins_per_function_select = gpio_register_width / 3; // 10, each 3 bits => 1 pin (top 2 reserved)

// the number of pins per pull up/down register
inline constexpr int pins_per_pull_up_down = gpio_register_width / 2; // 16, each 2 bits => 1 pin

template <int PinMax, int LaneWidth = (32 / PinMax)>
void gpio_update(int pin, std::uintptr_t reg, std::uint32_t value) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  // get a mask of `LaneWidth` set bits
  constexpr auto mask = (1U << LaneWidth) - 1U;
  auto bit_offset = (pin % PinMax) * LaneWidth;
  auto new_value = kio::mmio_read(reg);

  new_value &= ~(mask << bit_offset);
  new_value |= (value << bit_offset);

  kio::mmio_write(reg, new_value);
}

void kio::gpio_set(int pin) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  auto reg = (pin >= pins_per_set_clear) ? kio::gpio_set0 : kio::gpio_set1;

  gpio_update<pins_per_set_clear>(pin, reg, 1);
}

void kio::gpio_clear(int pin) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  auto reg = (pin >= pins_per_set_clear) ? kio::gpio_clear0 : kio::gpio_clear1;

  gpio_update<pins_per_set_clear>(pin, reg, 0);
}

void kio::gpio_function(int pin, GPIOFunction value) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  constexpr frt::Array select_offsets{kio::gpio_function_select0,
      kio::gpio_function_select1,
      kio::gpio_function_select2,
      kio::gpio_function_select3,
      kio::gpio_function_select4,
      kio::gpio_function_select5};

  auto reg = select_offsets[pin / pins_per_function_select];

  gpio_update<pins_per_function_select>(pin, reg, static_cast<std::uint32_t>(value));
}

void kio::gpio_pull(int pin, kio::PullState state) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

#ifdef INDIGO_RPI_4
  constexpr frt::Array select_offsets{kio::gpio_pull_up_down0,
      kio::gpio_pull_up_down1,
      kio::gpio_pull_up_down2,
      kio::gpio_pull_up_down3};

  auto reg = select_offsets[pin / pins_per_pull_up_down];

  gpio_update<pins_per_pull_up_down>(pin, reg, static_cast<std::uint32_t>(state));

#else
  inline constexpr std::uintptr_t gpio_pull_up_down_enable = kio::gpio_base + 0x94;
  inline constexpr std::uintptr_t gpio_pull_up_down_clock0 = kio::gpio_base + 0x98;
  inline constexpr std::uintptr_t gpio_pull_up_down_clock1 = kio::gpio_base + 0x9C;

  auto reg = (pin > single_reg_pin_max) ? gpio_pull_up_down_clock0 : gpio_pull_up_down_clock1;

  // see pg 101 in BCM2837 datasheet
  kio::mmio_write(gpio_pull_up_down_enable, static_cast<std::uint32_t>(state));
  kio::sleep(150);
  kio::mmio_write(reg, 1 << (pin % single_reg_pin_max + 1));
  kio::sleep(150);
  kio::mmio_write(gpio_pull_up_down_enable, 0);
  kio::mmio_write(reg, 0);
#endif
}
