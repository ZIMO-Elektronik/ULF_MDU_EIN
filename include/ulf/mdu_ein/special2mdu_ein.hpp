// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert special command to MDU_EIN
///
/// \file   ulf/mdu_ein/special2mdu_ein.hpp
/// \author Jonas Gahlert
/// \date   07/05/2025

#pragma once

#include <algorithm>
#include <span>
#include <ztl/inplace_vector.hpp>
#include "utility.hpp"

namespace ulf::mdu_ein {

/// Convert special command to MDU_EIN
///
/// \param  command Command
/// \param  value   Value
/// \return MDU_EIN special command frame
constexpr ztl::inplace_vector<uint8_t, max_special_frame_length>
special2mdu_ein(Command command,
                uint8_t subcommand,
                std::span<uint8_t const, 16uz> payload) {
  ztl::inplace_vector<uint8_t, max_special_frame_length> result{};
  auto iter{std::back_inserter(result)};

  std::ranges::copy(prefix, iter);                        // Prefix
  iter = uint16_2data(0x00uz, iter);                      // Length
  iter = uint32_2data(std::to_underlying(command), iter); // Command
  *iter++ = subcommand;                                   // Subcommand
  std::ranges::copy(payload, iter);                       // Payload
  std::ranges::copy(suffix, iter);                        // Suffix

  return result;
}

} // namespace ulf::mdu_ein
