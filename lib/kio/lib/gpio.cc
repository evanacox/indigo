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

inline constexpr int single_reg_pin_max = 31;
inline constexpr int pins_per_function_select = 10;
inline constexpr int all_alternates_set = 0b111;
inline constexpr int function_select_width = 3;
inline constexpr int pins_per_pull_up_down = 16;
inline constexpr int pull_up_down_width = 2;
inline constexpr int pull_down_all_set = 0b11;

template <int PinMax, int LaneWidth, int AllAlternatesSet = 0, bool ClearBefore = false>
void gpio_update(int pin, std::uintptr_t reg, std::uint32_t value) {
  auto bit_offset = (pin % PinMax) * LaneWidth;
  auto old_value = kio::mmio_read(reg);

  if constexpr (ClearBefore) {
    old_value &= ~(AllAlternatesSet << bit_offset);
  }

  // clear and then set the function flags
  kio::mmio_write(reg, old_value | (value << bit_offset));
}

void kio::gpio_set(int pin) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  auto reg = (pin > single_reg_pin_max) ? kio::gpio_set0 : kio::gpio_set1;
  auto bit_offset = pin % (single_reg_pin_max + 1); // 32nd pin -> 0th bit
  auto old_value = kio::mmio_read(reg);

  // set `bit_offset`th bit, if it's already set we're fine
  kio::mmio_write(reg, old_value | (1 << bit_offset));
}

void kio::gpio_clear(int pin) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

  auto reg = (pin > single_reg_pin_max) ? kio::gpio_clear0 : kio::gpio_clear1;
  auto bit_offset = pin % (single_reg_pin_max + 1); // 32nd pin -> 0th bit
  auto old_value = kio::mmio_read(reg);

  // clear `bit_offset`th bit, if it's already clear we're fine
  kio::mmio_write(reg, old_value & ~(1 << bit_offset));
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
  auto bit_offset = (pin % pins_per_function_select) * function_select_width;
  auto old_value = kio::mmio_read(reg);
  auto cleared = old_value & ~(all_alternates_set << bit_offset);

  // clear and then set the function flags
  kio::mmio_write(reg, cleared | (static_cast<std::uint32_t>(value) << bit_offset));
}

void kio::gpio_pull(int pin, kio::PullState state) {
  FRT_ASSERT(pin <= kio::gpio_max_pin, "must use existing pins");

#ifdef INDIGO_RPI_4
  constexpr frt::Array select_offsets{kio::gpio_pull_up_down0,
      kio::gpio_pull_up_down1,
      kio::gpio_pull_up_down2,
      kio::gpio_pull_up_down3};

  auto reg = select_offsets[pin / pins_per_pull_up_down];
  auto bit_offset = (pin % pins_per_pull_up_down) * pull_up_down_width;

  kio::mmio_write(reg, (static_cast<std::uint32_t>(state) << bit_offset));

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
