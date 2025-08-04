#include <gtest/gtest.h>

#include <algorithm>
#include <mdu/mdu.hpp>
#include <ulf/mdu_ein.hpp>
#include "../frame_builder.hpp"

TEST(special2mdu_ein, convert_speed_command) {
  auto frame = FrameBuilder::makeSpeedFrame(00uz).frame();
  auto res = ulf::mdu_ein::special2mdu_ein(
    ulf::mdu_ein::Command::Speed, 0x00uz, dummy_payload);

  ASSERT_TRUE(std::ranges::equal(frame, res));
}

TEST(special2mdu_ein, convert_entry_command) {
  auto frame = FrameBuilder::makeMDUALTFrame().frame();
  auto res = ulf::mdu_ein::special2mdu_ein(
    ulf::mdu_ein::Command::Entry, 0x00uz, dummy_payload);

  ASSERT_TRUE(std::ranges::equal(frame, res));
}
