// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert MDU_EIN to special command
///
/// \file   ulf/mdu_ein/mdu_ein2special.hpp
/// \author Jonas Gahlert
/// \date   26/03/2025

#pragma once

#include <algorithm>
#include <cstdint>
#include <expected>
#include <optional>
#include <system_error>
#include "special.hpp"
#include "utility.hpp"

namespace ulf::mdu_ein::detail {

using std::operator""sv;

/// Convert MDU_EIN to special command
///
/// \param  frame         MDU_EIN frame
/// \retval std::errc     Error
/// \retval std::nullopt  Incomplete Frame
/// \retval Special       Special Command
constexpr std::expected<std::optional<Special>, std::errc>
mdu_ein2special(std::span<uint8_t const> frame) {
  size_t const count{size(frame)};
  std::string_view f_str{std::bit_cast<char*>(data(frame)), size(frame)};

  // String must start with "MDUB"
  if (!f_str.starts_with(prefix.substr(0uz, count)))
    return std::unexpected{std::errc::invalid_argument};

  // Check if packet already contains length
  if (count < 6uz) return std::nullopt;

  auto it{begin(frame) + prefix.size()};

  // Check packet length
  size_t const length{data2uint16(it)};
  it += sizeof(uint16_t);

  if (length != 0u) return std::unexpected{std::errc::invalid_argument};

  // String must end with "MDUE"
  if (!f_str.substr(0u, special_frame_length)
         .ends_with(

           suffix.substr(0uz,
                         count - (size(prefix) + sizeof(uint16_t) +
                                  sizeof(Special::command) +
                                  sizeof(Special::subcommand) + payload_size))))
    return std::unexpected{std::errc::invalid_argument};

  // Check
  if (size(f_str) < special_frame_length) return std::nullopt;

  // Convert command and value
  uint32_t const command{data2uint32(it)};
  it += sizeof(uint32_t);

  switch (command) {
    case std::to_underlying(Command::Entry): [[fallthrough]];
    case std::to_underlying(Command::Speed): {
      Special special{.command{static_cast<Command>(command)},
                      .subcommand{*it++},
                      .payload{}};
      std::ranges::copy(
        frame.subspan(static_cast<size_t>(it - begin(frame)), payload_size),
        std::back_inserter(special.payload));
      return special;
    }
    default: return std::unexpected{std::errc::invalid_argument};
  }

  // No Match
  return std::unexpected{std::errc::invalid_argument};
}

} // namespace ulf::mdu_ein::detail
