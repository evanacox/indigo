//======---------------------------------------------------------------======//
//                                                                           //
// Copyright 2021-2022 Evan Cox <evanacox00@gmail.com>. All rights reserved. //
//                                                                           //
// Use of this source code is governed by a BSD-style license that can be    //
// found in the LICENSE.txt file at the root of this project, or at the      //
// following link: https://opensource.org/licenses/BSD-3-Clause              //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./device_tree.h"
#include "frt/core/bit.h"
#include "klog/print.h"

const indigo::FlattenedDevicetreeHeader& indigo::parse_dtb_header(std::uint64_t dtb) {
  // NOLINTNEXTLINE(performance-no-int-to-ptr)
  auto* header = reinterpret_cast<indigo::FlattenedDevicetreeHeader*>(dtb);

  // need to convert everything from big-endian to little-endian
  header->magic = frt::byte_swap(header->magic);
  header->total_size = frt::byte_swap(header->total_size);
  header->off_dt_struct = frt::byte_swap(header->off_dt_struct);
  header->off_dt_strings = frt::byte_swap(header->off_dt_strings);
  header->off_mem_rsvmap = frt::byte_swap(header->off_mem_rsvmap);
  header->version = frt::byte_swap(header->version);
  header->last_comp_version = frt::byte_swap(header->last_comp_version);
  header->boot_cpuid_phys = frt::byte_swap(header->boot_cpuid_phys);
  header->size_dt_strings = frt::byte_swap(header->size_dt_strings);
  header->size_dt_struct = frt::byte_swap(header->size_dt_struct);

  return *header;
}

void indigo::parse_dtb(std::uint64_t dtb) {
  const auto& header = indigo::parse_dtb_header(dtb);

  klog::writeln("got devicetree! from header: ");
  klog::writeln("  magic: ", header.magic, '(', klog::Hex(header.magic), ')');
  klog::writeln("  total_size: ", header.total_size);
  klog::writeln("  off_dt_struct: ", header.off_dt_struct);
  klog::writeln("  off_dt_strings: ", header.off_dt_strings);
  klog::writeln("  off_mem_rsvmap: ", header.off_mem_rsvmap);
  klog::writeln("  version: ", header.version);
  klog::writeln("  last_comp_version: ", header.last_comp_version);
  klog::writeln("  boot_cpuid_phys: ", header.boot_cpuid_phys);
  klog::writeln("  size_dt_strings: ", header.size_dt_strings);
  klog::writeln("  size_dt_struct: ", header.size_dt_struct);
}
