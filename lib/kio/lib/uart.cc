//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "kio/uart.h"
#include "kio/gpio.h"
#include "kio/mmio.h"
#include "kio/sleep.h"
#include <cstddef>
#include <cstdint>

namespace {
  inline constexpr int pull_none = 0;
  inline constexpr int function_alt5 = 2;
  inline constexpr int disable_interrupts = 0xC6;
  inline constexpr int default_baud_rate = 115200;

  [[nodiscard]] constexpr int baud_rate(int desired) noexcept {
    constexpr int system_clock_freq = 25'000'000; // 250mhz

    // see 2.2.1 in BCM2711 datasheet
    return system_clock_freq / (8 * (desired + 1));
  }

  bool is_write_ready() {
    // 5th bit is set if there's at least one byte in queue
    return (kio::mmio_read(kio::aux_uart_line_status) & 0b1'0000) != 0;
  }

  void write_byte_blocking(char c) {
    while (!is_write_ready()) {}

    kio::mmio_write(kio::aux_uart_io, c);
  }
} // namespace

void kio::uart_init() {
  // need to set pull status for UART pins on boot
  kio::gpio_pull(14, PullState::none);
  kio::gpio_pull(15, PullState::none);

  kio::mmio_write(kio::aux_enables, 1);               // enable the Mini UART (and its registers)
  kio::mmio_write(kio::aux_uart_extra_control, 0);    // disable receiver/transmitter temporarily
  kio::mmio_write(kio::aux_uart_interrupt_enable, 0); // disable interrupts for transfer/receive

#ifdef INDIGO_RPI_4
  kio::mmio_write(kio::aux_uart_line_control, 0b1); // set 8-bit
#else
  kio::mmio_write(kio::aux_uart_line_control, 0b11); // set 8-bit
#endif

  kio::mmio_write(kio::aux_uart_modem_control, 0);                        // set RTS to always be high
  kio::mmio_write(kio::aux_uart_baud_rate, baud_rate(default_baud_rate)); // set baud-rate to 11520
  kio::mmio_write(kio::aux_uart_extra_control, 0b11);                     // enable transmitter/receiver
}

void kio::uart_write_raw(char c) {
  write_byte_blocking(c);
}

void kio::uart_write_raw(const char* message) {
  for (const auto* it = message; *it != '\0'; ++it) {
    if (*it == '\n') {
      kio::uart_write_raw('\r');
    }

    kio::uart_write_raw(*it);
  }
}

void kio::uart_write(char c) {
  if (c == '\n') {
    uart_write_raw("\r\n");
  } else {
    uart_write('\n');
  }
}

void kio::uart_write(const char* message) {
  for (const auto* it = message; *it != '\0'; ++it) {
    if (*it == '\n') {
      kio::uart_write_raw('\r');
    }

    kio::uart_write_raw(*it);
  }
}

void kio::uart_writeln(char c) {
  kio::uart_write_raw(c);
  kio::uart_write_raw("\r\n");
}

void kio::uart_writeln(const char* message) {
  kio::uart_write_raw(message);
  kio::uart_write_raw("\r\n");
}
