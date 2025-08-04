#include "frame_builder.hpp"

#include <mdu/packet.hpp>

#include <algorithm>
#include <array>

FrameBuilder& FrameBuilder::prefix() {
  std::ranges::copy(ulf::mdu_ein::prefix, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::length(uint16_t length) {
  auto it{std::back_inserter(_data)};
  ulf::mdu_ein::uint16_2data(length, it);

  return *this;
}

FrameBuilder& FrameBuilder::packet(std::span<uint8_t const> packet) {
  std::ranges::copy(packet, std::back_inserter(_data));
  return *this;
}

FrameBuilder& FrameBuilder::special(uint32_t command,
                                    uint8_t subcommand,
                                    std::span<uint8_t const, 16uz> payload) {
  auto it{std::back_inserter(_data)};
  ulf::mdu_ein::uint32_2data(command, it);
  *it++ = subcommand;
  std::ranges::copy(payload, it);
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

FrameBuilder FrameBuilder::makeDCCZPPFrame(uint32_t sn) {
  std::array<uint8_t, ulf::mdu_ein::payload_size> payload{};
  auto it{begin(payload)};
  ulf::mdu_ein::uint32_2data(sn, it);
  return makeSpecialFrame(
    std::to_underlying(ulf::mdu_ein::Command::Entry), 0x02u, payload);
}

FrameBuilder FrameBuilder::makeDCCZSUFrame(uint32_t id, uint32_t sn) {
  std::array<uint8_t, ulf::mdu_ein::payload_size> payload{};
  auto it{begin(payload)};
  ulf::mdu_ein::uint32_2data(id, it);
  ulf::mdu_ein::uint32_2data(sn, it);

  return makeSpecialFrame(
    std::to_underlying(ulf::mdu_ein::Command::Entry), 0x01u, payload);
}

FrameBuilder FrameBuilder::makeMDUALTFrame() {
  std::array<uint8_t, ulf::mdu_ein::payload_size> payload{};
  return makeSpecialFrame(
    std::to_underlying(ulf::mdu_ein::Command::Entry), 0x00u, payload);
}

FrameBuilder FrameBuilder::makeSpeedFrame(uint8_t speed) {
  std::array<uint8_t, ulf::mdu_ein::payload_size> payload{};
  return makeSpecialFrame(
    std::to_underlying(ulf::mdu_ein::Command::Speed), speed, payload);
}

FrameBuilder
FrameBuilder::makeSpecialFrame(uint32_t command,
                               uint8_t subcommand,
                               std::span<uint8_t const, 16uz> payload) {
  FrameBuilder ret{};

  ret.prefix();
  ret.length();
  ret.special(command, subcommand, payload);
  ret.suffix();

  return ret;
}
