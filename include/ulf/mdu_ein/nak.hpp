// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Not acknowledge
///
/// \file   ulf/mdu_ein/nak.hpp
/// \author Vincent Hamp
/// \date   18/08/2024

#pragma once

#include <cstdint>

namespace ulf::mdu_ein {

inline constexpr uint8_t nak{0x15u};

} // namespace ulf::mdu_ein
