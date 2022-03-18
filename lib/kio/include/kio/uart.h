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

#include "./mmio.h"

namespace kio {
  inline constexpr std::uintptr_t aux_base = /* kio::mmio_base + */ 0x215000;
  inline constexpr std::uintptr_t aux_interrupt_status = kio::aux_base + 0x00;
  inline constexpr std::uintptr_t aux_enables = kio::aux_base + 0x04;
  inline constexpr std::uintptr_t aux_uart_io = kio::aux_base + 0x40;
  inline constexpr std::uintptr_t aux_uart_interrupt_enable = kio::aux_base + 0x44;
  inline constexpr std::uintptr_t aux_uart_interrupt_identify = kio::aux_base + 0x48;
  inline constexpr std::uintptr_t aux_uart_line_control = kio::aux_base + 0x4c;
  inline constexpr std::uintptr_t aux_uart_modem_control = kio::aux_base + 0x50;
  inline constexpr std::uintptr_t aux_uart_line_status = kio::aux_base + 0x54;
  inline constexpr std::uintptr_t aux_uart_modem_status = kio::aux_base + 0x58;
  inline constexpr std::uintptr_t aux_uart_extra_control = kio::aux_base + 0x58;
  inline constexpr std::uintptr_t aux_uart_baudrate = kio::aux_base + 0x68;
  inline constexpr int uart_transmit_pin = 14;
  inline constexpr int uart_receive_pin = 15;

  void uart_init();

  void uart_write_raw(char c);

  void uart_write_raw(const char* message);

  void uart_write(char c);

  void uart_write(const char* message);

  void uart_writeln(char c);

  void uart_writeln(const char* message);

  template <typename... Args> void uart_writeln(Args&&... args) {
    (kio::uart_write_raw(args), ...);
    kio::uart_write_raw("\r\n");
  }
} // namespace kio