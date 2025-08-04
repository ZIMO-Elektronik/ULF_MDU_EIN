// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Special command
///
/// \file   ulf/mdu_ein/special.hpp
/// \author Jonas Gahlert
/// \date   26/03/2025

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

namespace ulf::mdu_ein {

using std::operator""sv;

constexpr uint8_t payload_size{16u};

/// Command codes
enum class Command : uint32_t {
  Entry = std::ranges::fold_left(
    "ETRY"sv, 0u, [](uint32_t lhs, uint32_t rhs) { return (lhs << 8u | rhs); }),
  Speed = std::ranges::fold_left(
    "SPDS"sv, 0u, [](uint32_t lhs, uint32_t rhs) { return (lhs << 8u | rhs); }),
};

/// Special command
struct Special {

  Command command;
  uint8_t subcommand;
  ztl::inplace_vector<uint8_t, payload_size> payload;
};

} // namespace ulf::mdu_ein
