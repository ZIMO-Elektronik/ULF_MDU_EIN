// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert bytes to MDU_EIN
///
/// \file   ulf/mdu_ein/packet2mdu_ein.hpp
/// \author Jonas Gahlert
/// \date   07/05/2025

#pragma once

#include <algorithm>
#include <span>
#include <ztl/inplace_vector.hpp>
#include "utility.hpp"

namespace ulf::mdu_ein {

/// Convert bytes to MDU_EIN
///
/// \param  bytes Packet data as view
/// \return MDU_EIN packet frame
constexpr ztl::inplace_vector<uint8_t, max_packet_frame_length>
bytes2mdu_ein(std::span<uint8_t const> bytes) {
  assert(size(bytes) <= MDU_MAX_PACKET_SIZE);

  ztl::inplace_vector<uint8_t, max_packet_frame_length> result{};
  auto iter{std::back_inserter(result)};

  std::ranges::copy(prefix, iter);                        // Prefix
  uint16_2data(static_cast<uint16_t>(size(bytes)), iter); // Length
  std::ranges::copy(bytes, iter);                         // Data
  std::ranges::copy(suffix, iter);                        // Suffix

  return result;
}

} // namespace ulf::mdu_ein
