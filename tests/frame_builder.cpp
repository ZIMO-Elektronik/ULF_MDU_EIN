#include "frame_builder.hpp"

#include <mdu/packet.hpp>

#include <algorithm>
#include <array>

FrameBuilder& FrameBuilder::prefix() {
  std::ranges::copy(ulf::mdu_ein::prefix, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::length(uint16_t length) {
  // Convert length to bytes
  std::array<uint8_t, 2uz> l_data{};
  ulf::mdu_ein::uint16_2data(length, std::data(l_data));

  // Copy length and data
  std::ranges::copy(l_data, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::packet(std::span<uint8_t const> packet) {
  std::ranges::copy(packet, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::special(uint32_t command, uint16_t value) {
  std::array<uint8_t, 6uz> special{};
  ulf::mdu_ein::uint32_2data(command, std::data(special));
  ulf::mdu_ein::uint16_2data(value, std::data(special) + 4);
  std::ranges::copy(special, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::suffix() {
  std::ranges::copy(ulf::mdu_ein::suffix, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::data(std::span<uint8_t const> data) {
  std::ranges::copy(data, std::back_inserter(_data));
  return *this;
}

Frame FrameBuilder::frame() { return _data; }

std::span<uint8_t> FrameBuilder::bytes() { return std::span<uint8_t>{_data}; }

FrameBuilder FrameBuilder::makeBusyPacketFrame() {
  return makePacketFrame(::mdu::make_busy_packet());
}

FrameBuilder FrameBuilder::makePingPacketFrame(uint8_t decoder_id) {
  return makePacketFrame(::mdu::make_ping_packet(decoder_id));
}

FrameBuilder FrameBuilder::makePacketFrame(::mdu::Packet const& packet) {
  FrameBuilder ret;

  ret.prefix();
  ret.length(size(packet));
  ret.packet(packet);
  ret.suffix();

  return ret;
}

FrameBuilder FrameBuilder::makeDCCZPPFrame() {
  return makeSpecialFrame(std::to_underlying(ulf::mdu_ein::Command::Entry),
                          0x02u);
}

FrameBuilder FrameBuilder::makeDCCZSUFrame() {
  return makeSpecialFrame(std::to_underlying(ulf::mdu_ein::Command::Entry),
                          0x01u);
}

FrameBuilder FrameBuilder::makeMDUALTFrame() {
  return makeSpecialFrame(std::to_underlying(ulf::mdu_ein::Command::Entry),
                          0x00u);
}

FrameBuilder FrameBuilder::makeSpecialFrame(uint32_t command, uint16_t value) {
  FrameBuilder ret{};

  ret.prefix();
  ret.length();
  ret.special(command, value);
  ret.suffix();

  return ret;
}
