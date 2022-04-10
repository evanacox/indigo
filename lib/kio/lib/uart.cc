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
    return (kio::SerialPort{}.line_status.read() & 0b10'0000) != 0;
  }

  void use_pin_as_alt5(int pin) {
    // need to set pull status for UART pins on boot, and switch them to alt5 function
    kio::gpio_pull(pin, kio::PullState::none);
    kio::gpio_function(pin, kio::GPIOFunction::alternate_5);
  }
} // namespace

void kio::uart_init() {
  constexpr std::uint32_t clear_recv_transmit = 0b1100'0110;
  kio::SerialPort serial;

  serial.enabled = 1;                              // enable the Mini UART (and its registers)
  serial.interrupt_enable = 1;                     // disable interrupts
  serial.extra_control = 0;                        // disable receiver/transmitter temporarily
  serial.line_control = 0b11;                      // set to use 8-bit
  serial.modem_control = 0;                        // set RTS line to always be high
  serial.interrupt_enable = 0;                     // disable interrupts (again?)
  serial.interrupt_identify = clear_recv_transmit; // clear recv/transmit FIFO. why is this part of this register?
  serial.baudrate = baudrate(default_baudrate);    // set baud-rate to 11520

  use_pin_as_alt5(kio::uart_transmit_pin); // switch pin to UART1 mode
  use_pin_as_alt5(kio::uart_receive_pin);  // switch pin to UART1 mode

  serial.extra_control = 0b11; // enable transmitter/receiver
}

void kio::uart_write_raw(char c) {
  while (!is_write_ready()) {
    /* need to block until UART is done transmitting/receiving and is ready for a byte */
  }

  kio::SerialPort{}.io = static_cast<std::uint32_t>(c);
}

void kio::uart_write_raw(const char* message) {
  for (const auto* it = message; *it != '\0'; ++it) {
    if (*it == '\n') {
      kio::uart_write_raw('\r');
    }

    kio::uart_write_raw(*it);
  }
}