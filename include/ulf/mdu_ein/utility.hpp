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
  size(prefix)                  // Prefix
  + sizeof(uint16_t)            // Length
  + sizeof(Special::command)    // Command
  + sizeof(Special::subcommand) // Subcommand
  + payload_size                // Payload
  + size(suffix)                // Suffix
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
  size(prefix)                  // Prefix length
  + sizeof(uint16_t)            // Length length
  + sizeof(Special::command)    // Command length
  + sizeof(Special::subcommand) // Subcommand length
  + payload_size                // Payload length
  + size(suffix)                // Suffix length
};

using ::mdu::data2uint16, ::mdu::data2uint32, ::mdu::uint16_2data,
  ::mdu::uint32_2data;

} // namespace ulf::mdu_ein
