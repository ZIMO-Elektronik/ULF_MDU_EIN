// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert MDU_EIN to bytes or packet
///
/// \file   ulf/mdu_ein/mdu_ein2packet.hpp
/// \author Jonas Gahlert
/// \date   26/03/2025

#pragma once

#include <algorithm>
#include <expected>
#include <mdu/mdu.hpp>
#include <optional>
#include <span>
#include <string_view>
#include <system_error>
#include "mdu_ein2special.hpp"
#include "utility.hpp"

namespace ulf::mdu_ein {

/// Convert MDU_EIN to bytes
///
/// \bug
/// With the current setup, a non-matching packet length is not caught. Not a
/// problem if the length is too small but if it is larger than the actual
/// frame, the next frame could also be matched (and could cause it to be
/// marked as invalid as well)
///
/// \param  frame         MDU_EIN frame
/// \retval RefOutput     Success
/// \retval std::nullopt  Insufficient data
/// \retval std::errc     Error
constexpr std::expected<std::optional<RefOutput>, std::errc>
mdu_ein2bytes(std::span<uint8_t const> frame) {
  size_t const count{size(frame)};
  RefOutput res{};

  std::string_view f_str{std::bit_cast<char*>(data(frame)), size(frame)};

  // String must start with "MDUB"
  if (!f_str.starts_with(prefix.substr(0uz, count)))
    return std::unexpected{std::errc::invalid_argument};

  // Check if packet already contains length
  if (count < 6uz) return std::nullopt;

  auto const length{data2uint16(&frame[4])};

  if (length == 0u) {
    // Special Command
    return detail::mdu_ein2special(frame);
  }

  // Check packet length
  if (size(frame) < packet_length2frame_length(length)) return std::nullopt;

  // String must end with "MDUE"
  if (!f_str.substr(0u, packet_length2frame_length(length))
         .ends_with(suffix.substr(
           0uz, count - (length + size(prefix) + sizeof(uint16_t)))))
    return std::unexpected{std::errc::invalid_argument};

  // Return span to packet
  res = frame.subspan(size(prefix) + sizeof(uint16_t), length);
  return res;
}

/// Convert MDU_EIN to packet
///
/// \param  frame         MDU_EIN frame
/// \retval CopyOutput    Success
/// \retval std::nullopt  Insufficient data
/// \retval std::errc     Error
constexpr std::expected<std::optional<CopyOutput>, std::errc>
mdu_ein2packet(std::span<uint8_t const> frame) {
  // Parse and check result
  auto ref{mdu_ein2bytes(frame)};
  if (!ref) return std::unexpected{ref.error()}; // Invalid argument
  else if (!*ref) return std::nullopt;           // Insufficient data

  // Copy packet
  CopyOutput res{};

  if (std::holds_alternative<PacketRef>(**ref))
    // Copy packet
    std::ranges::copy(std::get<PacketRef>(**ref),
                      std::back_inserter(std::get<mdu::Packet>(res)));
  else
    // Copy special
    res.emplace<Special>(std::get<Special>(**ref));

  return res;
}

} // namespace ulf::mdu_ein
