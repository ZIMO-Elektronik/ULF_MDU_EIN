#include <gtest/gtest.h>

#include <ulf/mdu_ein.hpp>
#include "../frame_builder.hpp"

TEST(packet, valid_frame) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);

  auto eval{::mdu::make_busy_packet()};

  ASSERT_TRUE(std::holds_alternative<mdu::Packet>(**res));

  auto r = std::get<mdu::Packet>(**res).begin();
  auto e = eval.begin();

  while (e != eval.end()) { ASSERT_EQ(*r++, *e++); }

  ASSERT_EQ(r, std::get<mdu::Packet>(**res).end());
}

TEST(packet, valid_data_frame) {
  std::vector<uint8_t> data(64uz);
  auto packet{mdu::make_zsu_update_packet(0u, std::span<uint8_t, 64>{data})};
  auto frame{FrameBuilder::makePacketFrame(packet).frame()};

  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);

  ASSERT_TRUE(std::holds_alternative<mdu::Packet>(**res));

  auto r = std::get<mdu::Packet>(**res).begin();
  auto e = packet.begin();

  while (e != packet.end()) { ASSERT_EQ(*r++, *e++); }

  ASSERT_EQ(r, std::get<mdu::Packet>(**res).end());
}

TEST(packet, prefix) {
  FrameBuilder builder{};
  auto frame{builder.prefix().frame()};
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(packet, length) {
  FrameBuilder builder{};
  auto frame{builder.prefix().length(4u).frame()};
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(packet, packet) {
  FrameBuilder builder{};
  auto packet{::mdu::make_busy_packet()};
  auto frame{builder.prefix().length(size(packet)).packet(packet).frame()};
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(packet, faulty_prefix) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  frame.at(3u) = 0u;
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_FALSE(res);
}

TEST(packet, faulty_length) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  frame.at(4u) = 0u;
  frame.at(5u) = 1u;
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_FALSE(res);
}

TEST(packet, faulty_suffix) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  frame.back() = 0u;
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_FALSE(res);
}

TEST(packet, parse_special) {
  auto frame{FrameBuilder::makeMDUALTFrame().frame()};
  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);

  ASSERT_TRUE(std::holds_alternative<ulf::mdu_ein::Special>(**res));
  ASSERT_EQ(std::get<ulf::mdu_ein::Special>(**res).command,
            ulf::mdu_ein::Command::Entry);
  ASSERT_EQ(std::get<ulf::mdu_ein::Special>(**res).subcommand, 0x00u);
  ASSERT_TRUE(std::ranges::equal(std::get<ulf::mdu_ein::Special>(**res).payload,
                                 dummy_payload));
}

TEST(packet, additional_data) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  std::fill_n(std::back_inserter(frame), 10u, 0u);

  auto res{ulf::mdu_ein::mdu_ein2packet(frame)};

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);

  auto eval{::mdu::make_busy_packet()};

  ASSERT_TRUE(std::holds_alternative<mdu::Packet>(**res));

  auto r{std::get<mdu::Packet>(**res).begin()};
  auto e{eval.begin()};

  while (e != eval.end()) { ASSERT_EQ(*r++, *e++); }

  ASSERT_EQ(r, std::get<mdu::Packet>(**res).end());
}
