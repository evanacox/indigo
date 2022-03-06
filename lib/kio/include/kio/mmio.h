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

  /// Gets the MMIO starting address, can be offset to get a specific MMIO peripheral
  ///
  /// \return `kio::mmio_base` as a pointer
  [[nodiscard]] inline std::byte* mmio_address() {
    // technically this is not `constexpr` due to standard requiring pointer bit-casts
    // to not be constant expressions, unfortunate
    return std::bit_cast<std::byte*>(kio::mmio_base);
  }

  /// Writes to a specific MMIO offset
  ///
  /// \param offset The offset from `mmio_address()` to write to
  /// \param data The data to write to the MMIO address
  inline void mmio_write(std::uintptr_t offset, std::uint32_t data) {
    *reinterpret_cast<volatile std::uint32_t*>(mmio_address() + offset) = data;
  }

  /// Reads from a specific MMIO offset
  ///
  /// \param offset The offset from `mmio_read()` to read from
  /// \return The data at that MMIO address
  inline std::uint32_t mmio_read(std::uintptr_t offset) {
    return *reinterpret_cast<volatile std::uint32_t*>(kio::mmio_address() + offset);
  }
} // namespace kio