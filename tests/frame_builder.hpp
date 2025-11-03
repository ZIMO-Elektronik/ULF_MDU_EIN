#pragma once

#include <ulf/mdu_ein.hpp>

#include <span>
#include <string_view>
#include <vector>

using Frame = std::vector<uint8_t>;

constexpr std::array<uint8_t, 16uz> dummy_payload{};

class FrameBuilder {
public:
  // Add prefix
  FrameBuilder& prefix();

  // Add length
  FrameBuilder& length(uint16_t length = 0u);

  // Add packet
  FrameBuilder& packet(std::span<uint8_t const> packet);

  // Add special
  FrameBuilder& special(uint32_t command,
                        uint8_t subcommand,
                        std::span<uint8_t const, 16uz> payload);

  // Add suffix
  FrameBuilder& suffix();

  // Add raw data
  FrameBuilder& data(std::span<uint8_t const> data);

  // Getter
  Frame frame();
  std::span<uint8_t> bytes();

  // Builder methods
  static FrameBuilder makeBusyPacketFrame();
  static FrameBuilder makePingPacketFrame(uint8_t decoder_id = 0u);
  static FrameBuilder makePacketFrame(::mdu::Packet const& packet);
  static FrameBuilder makeDCCZPPFrame(uint32_t sn = 0uz);
  static FrameBuilder makeDCCZSUFrame(uint32_t id = 0uz, uint32_t sn = 0uz);
  static FrameBuilder makeMDUALTFrame();
  static FrameBuilder makeSpeedFrame(uint8_t speed);
  static FrameBuilder makeSpecialFrame(uint32_t command,
                                       uint8_t subcommand,
                                       std::span<uint8_t const, 16uz> payload);

private:
  Frame _data{};
};
