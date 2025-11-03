#include <ulf/mdu_ein.hpp>

void mdu_ein2packet(auto&& mdu_ein_frame) {
  auto maybe_mdu{ulf::mdu_ein::mdu_ein2packet(mdu_ein_frame)};

  // Could be MDU
  if (maybe_mdu) {
    // Already complete?
    if (*maybe_mdu) {
      // Packet
      if (std::holds_alternative<mdu::Packet>(**maybe_mdu))
        auto packet{std::get<mdu::Packet>(**maybe_mdu)};
      // ... or special command
      else auto special{std::get<ulf::mdu_ein::Special>(**maybe_mdu)};
    }
    // No, still missing data
    else {}
  }
  // Error, not MDU
  else {}
}

void mdu_ein2bytes(auto&& mdu_ein_frame) {
  auto maybe_mdu{ulf::mdu_ein::mdu_ein2bytes(mdu_ein_frame)};

  // Could be MDU
  if (maybe_mdu) {
    // Already complete?
    if (*maybe_mdu) {
      // PacketRef
      if (std::holds_alternative<ulf::mdu_ein::PacketRef>(**maybe_mdu))
        auto packet_ref{std::get<ulf::mdu_ein::PacketRef>(**maybe_mdu)};
      // ... or special command
      else auto special{std::get<ulf::mdu_ein::Special>(**maybe_mdu)};
    }
    // No, still missing data
    else {}
  }
  // Error, not MDU
  else {}
}

int main() {
  std::vector<uint8_t> mdu_ein_frame{0x4Du,
                                     0x44u,
                                     0x55u,
                                     0x42u,
                                     0x00u,
                                     0x05u,
                                     0xFFu,
                                     0xFFu,
                                     0xFFu,
                                     0xF2u,
                                     0x70u,
                                     0x4Du,
                                     0x44u,
                                     0x55u,
                                     0x45u};
  std::vector<uint8_t> special{
    0x4Du, 0x44u, 0x55u, 0x42u, 0x00u, 0x00u, 0x45u, 0x54u, 0x52u, 0x59u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x4Du, 0x44u, 0x55u, 0x45u};
  mdu_ein2packet(mdu_ein_frame);
  mdu_ein2packet(special);
  mdu_ein2bytes(mdu_ein_frame);
  mdu_ein2bytes(special);
}
