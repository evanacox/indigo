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

#include <bit>
#include <cstddef>
#include <cstdint>

namespace kio {
  /// Offset for Raspberry Pi 4 for MMIO hardware. Address depends on board
  inline constexpr std::uintptr_t mmio_base =
#ifdef INDIGO_RPI_3
      0x3F000000;
#elif INDIGO_RPI_4
      0xFE000000;
#else
#error "Must define `INDIGO_RPI_3` or `INDIGO_RPI_4`!"
#endif

  /// Writes to a specific MMIO offset
  ///
  /// \param address The address to write to
  /// \param data The data to write to the MMIO address
  inline void mmio_write(std::uintptr_t address, std::uint32_t data) {
    *std::bit_cast<volatile std::uint32_t*>(address) = data;
  }

  /// Reads from a specific MMIO offset
  ///
  /// \param address The address to read from
  /// \return The data at that MMIO address
  inline std::uint32_t mmio_read(std::uintptr_t address) {
    return *std::bit_cast<volatile std::uint32_t*>(address);
  }
} // namespace kio