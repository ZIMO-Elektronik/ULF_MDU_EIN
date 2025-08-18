// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Convert MDU feedback to MDU_EIN response
///
/// \file   ulf/mdu_ein/response2mdu_ein.hpp
/// \author Jonas Gahlert
/// \date   30/04/2025

#pragma once

#include <ztl/inplace_vector.hpp>
#include "ack.hpp"
#include "nak.hpp"

namespace ulf::mdu_ein {

/// Response type
using Response = ztl::inplace_vector<uint8_t, 4uz>;

/// Response separator (ASCII semicolon)
constexpr uint8_t separator{static_cast<uint8_t>(';')};

/// Response end character (ASCII colon)
constexpr uint8_t end{static_cast<uint8_t>(':')};

/// Formats a MDU_EIN response
///
/// \param  ch1 Channel 1 feedback
/// \param  ch2 Channel 2 feedback
/// \return Formatted response
constexpr Response response2mdu_ein(bool ch1, bool ch2) {
  Response retval{};
  retval.push_back(ch1 ? ack : nak);
  retval.push_back(separator);
  retval.push_back(ch2 ? ack : nak);
  retval.push_back(end);
  return retval;
}

} // namespace ulf::mdu_ein
