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

#include <cstdint>

namespace indigo {
  struct FlattenedDevicetreeHeader {
    std::uint32_t magic;
    std::uint32_t total_size;
    std::uint32_t off_dt_struct;
    std::uint32_t off_dt_strings;
    std::uint32_t off_mem_rsvmap;
    std::uint32_t version;
    std::uint32_t last_comp_version;
    std::uint32_t boot_cpuid_phys;
    std::uint32_t size_dt_strings;
    std::uint32_t size_dt_struct;
  };

  /// Locates and parses the header for a Devicetree blob. This involves
  /// turning everything from big endian into little endian.
  ///
  /// \param dtb 32-bit pointer to the devicetree blob
  const FlattenedDevicetreeHeader& parse_dtb_header(std::uint64_t dtb);

  /// Parses a Devicetree blob.
  ///
  /// \param dtb 32-bit pointer to the devicetree blob
  void parse_dtb(std::uint64_t dtb);
} // namespace indigo