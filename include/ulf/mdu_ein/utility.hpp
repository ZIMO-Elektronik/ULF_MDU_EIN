// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Utility functions
///
/// \file   ulf/mdu_ein/util.hpp
/// \author Jonas Gahlert
/// \date   26/03/2025

#pragma once

#include <concepts>
#include <cstdint>
#include <iterator>
#include <mdu/mdu.hpp>
#include <variant>
#include "special.hpp"

namespace ulf::mdu_ein {

/// Prefix
inline constexpr std::string_view prefix{"MDUB"};

/// Suffix
inline constexpr std::string_view suffix{"MDUE"};

/// Maximum MDU_EIN packet frame length
inline constexpr size_t max_packet_frame_length{
  size(prefix)          // Prefix
  + sizeof(uint16_t)    // Length
  + MDU_MAX_PACKET_SIZE // Packet
  + size(suffix)        // Suffix
};

/// MDU_EIN special command frame length
inline constexpr size_t max_special_frame_length{
  size(prefix)       // Prefix
  + sizeof(uint16_t) // Length
  + sizeof(uint32_t) // Command
  + sizeof(uint16_t) // Value
  + size(suffix)     // Suffix
};

/// Packet reference
using PacketRef = std::span<mdu::Packet::value_type const>;

/// Parse output as copy
using CopyOutput = std::variant<mdu::Packet, Special>;

/// Parse output as reference (std::span)
using RefOutput = std::variant<PacketRef, Special>;

/// Packet length to frame length
///
/// \param  length  MDU packet length
/// \return MDU_EIN frame length
constexpr size_t packet_length2frame_length(size_t length) {
  return size(prefix)       // Prefix length
         + sizeof(uint16_t) // Length length
         + length           // Packet length
         + size(suffix);    // Suffix length
}

/// Special command frame length
///
/// \return MDU_EIN special command frame length
inline constexpr size_t special_frame_length{
  size(prefix)       // Prefix length
  + sizeof(uint16_t) // Length length
  + 6uz              // Command length
  + size(suffix)     // Suffix length
};

/// Data to uint16_t
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return uint16_t
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr auto data2uint16(RandomIt first) {
  return static_cast<uint16_t>(first[0uz] << 8u | first[1uz] << 0u);
}

/// uint16 to data
///
/// \tparam OutputIt  std::output_iterator
/// \param  hword     Half-word to convert
/// \param  out       Beginning of the destination range
/// \return Output iterator one past the last element copied
template<std::output_iterator<uint8_t> OutputIt>
constexpr auto uint16_2data(uint16_t hword, OutputIt out) {
  *out++ = static_cast<uint8_t>((hword & 0x0000'FF00u) >> 8u);
  *out++ = static_cast<uint8_t>((hword & 0x0000'00FFu) >> 0u);
  return out;
}

/// Data to uint32_t
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return uint32_t
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr auto data2uint32(RandomIt first) {
  return static_cast<uint32_t>(first[0uz] << 24u | first[1uz] << 16u |
                               first[2uz] << 8u | first[3uz] << 0u);
}

/// uint32_t to data
///
/// \tparam OutputIt  std::output_iterator
/// \param  word      Word to convert
/// \param  out       Beginning of the destination range
/// \return Output iterator one past the last element copied
template<std::output_iterator<uint8_t> OutputIt>
constexpr auto uint32_2data(uint32_t word, OutputIt out) {
  *out++ = static_cast<uint8_t>((word & 0xFF00'0000u) >> 24u);
  *out++ = static_cast<uint8_t>((word & 0x00FF'0000u) >> 16u);
  *out++ = static_cast<uint8_t>((word & 0x0000'FF00u) >> 8u);
  *out++ = static_cast<uint8_t>((word & 0x0000'00FFu) >> 0u);
}

} // namespace ulf::mdu_ein
