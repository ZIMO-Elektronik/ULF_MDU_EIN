#include <gtest/gtest.h>

#include <ulf/mdu_ein.hpp>

TEST(response, response) {
  bool channel1{true}, channel2{false};
  auto response{ulf::mdu_ein::response2mdu_ein(channel1, channel2)};

  // Match separator and end
  ASSERT_EQ(response[1], ulf::mdu_ein::separator);
  ASSERT_EQ(response[3], ulf::mdu_ein::end);

  // Match response channels
  ASSERT_EQ(response[0], ulf::mdu_ein::ack);
  ASSERT_EQ(response[2], ulf::mdu_ein::nak);
}
