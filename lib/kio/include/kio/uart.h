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
#include "frt/utility/io_port.h"

namespace kio {
  inline constexpr int uart_transmit_pin = 14;
  inline constexpr int uart_receive_pin = 15;
  inline static constexpr std::uintptr_t aux_base = kio::mmio_base + 0x215000;

  template <std::uintptr_t Base> struct MiniUART1 {
    inline static constexpr std::uintptr_t aux_interrupt_status = Base + 0x00;
    inline static constexpr std::uintptr_t aux_enables = Base + 0x04;
    inline static constexpr std::uintptr_t aux_uart_io = Base + 0x40;
    inline static constexpr std::uintptr_t aux_uart_interrupt_enable = Base + 0x44;
    inline static constexpr std::uintptr_t aux_uart_interrupt_identify = Base + 0x48;
    inline static constexpr std::uintptr_t aux_uart_line_control = Base + 0x4c;
    inline static constexpr std::uintptr_t aux_uart_modem_control = Base + 0x50;
    inline static constexpr std::uintptr_t aux_uart_line_status = Base + 0x54;
    inline static constexpr std::uintptr_t aux_uart_modem_status = Base + 0x58;
    inline static constexpr std::uintptr_t aux_uart_extra_control = Base + 0x58;
    inline static constexpr std::uintptr_t aux_uart_baudrate = Base + 0x68;

    frt::IOPortRW<std::uint32_t, aux_interrupt_status> interrupt_status;
    frt::IOPortRW<std::uint32_t, aux_enables> enabled;
    frt::IOPortRW<std::uint32_t, aux_uart_io> io;
    frt::IOPortRW<std::uint32_t, aux_uart_interrupt_enable> interrupt_enable;
    frt::IOPortRW<std::uint32_t, aux_uart_interrupt_identify> interrupt_identify;
    frt::IOPortRW<std::uint32_t, aux_uart_line_control> line_control;
    frt::IOPortRW<std::uint32_t, aux_uart_modem_control> modem_control;
    frt::IOPortRW<std::uint32_t, aux_uart_line_status> line_status;
    frt::IOPortRW<std::uint32_t, aux_uart_modem_status> modem_status;
    frt::IOPortRW<std::uint32_t, aux_uart_extra_control> extra_control;
    frt::IOPortRW<std::uint32_t, aux_uart_baudrate> baudrate;
  };

  using SerialPort = MiniUART1<aux_base>;

  void uart_init();

  void uart_write_raw(char c);

  void uart_write_raw(const char* message);
} // namespace kio