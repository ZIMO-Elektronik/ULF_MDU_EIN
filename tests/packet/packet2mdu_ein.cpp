#include <gtest/gtest.h>

#include <algorithm>
#include <mdu/mdu.hpp>
#include <ulf/mdu_ein.hpp>
#include "../frame_builder.hpp"

TEST(packet2mdu_ein, convert_busy_packet) {
  auto frame{FrameBuilder::makeBusyPacketFrame().frame()};
  auto packet{mdu::make_busy_packet()};
  auto res{ulf::mdu_ein::bytes2mdu_ein({packet})};

  ASSERT_TRUE(std::ranges::equal(frame, res));
}

TEST(packet2mdu_ein, conver_ping_packet) {
  auto frame{FrameBuilder::makePingPacketFrame().frame()};
  auto packet{mdu::make_ping_packet(0uz)};
  auto res{ulf::mdu_ein::bytes2mdu_ein({packet})};

  ASSERT_TRUE(std::ranges::equal(frame, res));
}
