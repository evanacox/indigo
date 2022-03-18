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
  inline constexpr int default_baudrate = 115200;

  [[nodiscard]] constexpr int baudrate(int desired_rate) noexcept {
    // constexpr int system_clock_freq = 25'000'000; // 250mhz
    constexpr int system_clock_freq2 = 500'000'000;

    // see 2.2.1 in BCM2711 datasheet. we're trying to calculate the value of
    // `baudrate_reg` so we rewrite the equation a bit
    return (system_clock_freq2 / (8 * desired_rate)) - 1;
  }

  bool is_write_ready() {
    // 6th bit is set if there's at least one byte in queue
    return (kio::mmio_read(kio::aux_uart_line_status) & 0b10'0000) != 0;
  }

  void use_pin_as_alt5(int pin) {
    // need to set pull status for UART pins on boot, and switch them to alt5 function
    kio::gpio_pull(pin, kio::PullState::none);
    kio::gpio_function(pin, kio::GPIOFunction::alternate_5);
  }
} // namespace

void kio::uart_init() {
  kio::mmio_write(kio::aux_enables, 1);                                // enable the Mini UART (and its registers)
  kio::mmio_write(kio::aux_uart_interrupt_enable, 0);                  // disable interrupts
  kio::mmio_write(kio::aux_uart_extra_control, 0);                     // disable receiver/transmitter temporarily
  kio::mmio_write(kio::aux_uart_line_control, 3);                      // set to use 8-bit
  kio::mmio_write(kio::aux_uart_modem_control, 0);                     // set RTS line to always be high
  kio::mmio_write(kio::aux_uart_interrupt_enable, 0);                  // disable interrupts (again?)
  kio::mmio_write(kio::aux_uart_interrupt_identify, 0b1100'0110);      // clear recv/transmit FIFO
  kio::mmio_write(kio::aux_uart_baudrate, baudrate(default_baudrate)); // set baud-rate to 11520
  use_pin_as_alt5(kio::uart_transmit_pin);                             // switch pin to UART1 mode
  use_pin_as_alt5(kio::uart_receive_pin);                              // switch pin to UART1 mode
  kio::mmio_write(kio::aux_uart_extra_control, 3);                     // enable transmitter/receiver
}

void kio::uart_write_raw(char c) {
  while (!is_write_ready()) {
    /* need to block until UART is done transmitting/receiving and is ready for a byte */
  }

  kio::mmio_write(kio::aux_uart_io, static_cast<std::uint32_t>(c));
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
