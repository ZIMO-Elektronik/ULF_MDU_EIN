#pragma once

#include <ulf/mdu_ein.hpp>

#include <span>
#include <string_view>
#include <vector>

using Frame = std::vector<uint8_t>;

class FrameBuilder {
public:
  // Add prefix
  FrameBuilder& prefix();

  // Add length
  FrameBuilder& length(uint16_t length = 0u);

  // Add packet
  FrameBuilder& packet(std::span<uint8_t const> packet);

  // Add special
  FrameBuilder& special(uint32_t command, uint16_t payload);

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
  static FrameBuilder makeDCCZPPFrame();
  static FrameBuilder makeDCCZSUFrame();
  static FrameBuilder makeMDUALTFrame();
  static FrameBuilder makeSpecialFrame(uint32_t command, uint16_t value);

private:
  Frame _data{};
};
