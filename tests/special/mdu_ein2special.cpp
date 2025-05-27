#include <gtest/gtest.h>

#include <ulf/mdu_ein.hpp>
#include "../frame_builder.hpp"

TEST(special, valid_dcczpp_frame) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);
  ASSERT_EQ((**res).command, ulf::mdu_ein::Command::Entry);
  ASSERT_EQ((**res).payload, 0x02u);
}

TEST(special, valid_dcczsu_frame) {
  auto frame = FrameBuilder::makeDCCZSUFrame().frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);
  ASSERT_EQ((**res).command, ulf::mdu_ein::Command::Entry);
  ASSERT_EQ((**res).payload, 0x01u);
}

TEST(special, valid_mdualt_frame) {
  auto frame = FrameBuilder::makeMDUALTFrame().frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);
  ASSERT_EQ((**res).command, ulf::mdu_ein::Command::Entry);
  ASSERT_EQ((**res).payload, 0x00u);
}

TEST(special, prefix) {
  auto builder = FrameBuilder{};
  auto frame = builder.prefix().frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(special, special) {
  auto builder = FrameBuilder{};
  auto frame =
    builder.prefix()
      .length()
      .special(std::to_underlying(ulf::mdu_ein::Command::Entry), 0x00uz)
      .frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(special, incomplete_prefix) {
  auto builder = FrameBuilder{};
  auto frame = builder.prefix().frame();
  auto res =
    ulf::mdu_ein::detail::mdu_ein2special(std::span{frame}.subspan(0uz, 2uz));

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(special, incomplete_suffix) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  auto res = ulf::mdu_ein::detail::mdu_ein2special(
    std::span{frame}.subspan(0uz, size(frame) - 2uz));

  ASSERT_TRUE(res);
  ASSERT_FALSE(*res);
}

TEST(special, faulty_prefix) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  frame.at(3u) = 0u;
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_FALSE(res);
}

TEST(special, faulty_length) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  frame.at(4u) = 54u;
  frame.at(5u) = 53u;
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_FALSE(res);
}

TEST(special, faulty_special) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  frame.at(7u) = 0u;
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_FALSE(res);
}

TEST(special, faulty_suffix) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  frame.back() = 0u;
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_FALSE(res);
}

TEST(special, additional_data) {
  auto frame = FrameBuilder::makeDCCZPPFrame().frame();
  std::fill_n(std::back_inserter(frame), 10u, 0u);
  auto res = ulf::mdu_ein::detail::mdu_ein2special(frame);

  ASSERT_TRUE(res);
  ASSERT_TRUE(*res);
  ASSERT_EQ((**res).command, ulf::mdu_ein::Command::Entry);
  ASSERT_EQ((**res).payload, 0x02u);
}
