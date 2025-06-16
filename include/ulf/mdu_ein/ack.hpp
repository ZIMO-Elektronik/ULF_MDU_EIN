// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Acknowledge
///
/// \file   ulf/mdu_ein/ack.hpp
/// \author Vincent Hamp
/// \date   18/08/2024

#pragma once

#include <cstdint>

namespace ulf::mdu_ein {

using Ack = uint8_t;

inline constexpr Ack ack{0x06u};

} // namespace ulf::mdu_ein
