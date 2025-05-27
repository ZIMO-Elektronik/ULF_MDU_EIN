#include <gtest/gtest.h>
#include <ulf/mdu_ein.hpp>
#include "random_container.hpp"

TEST(fuzz, mdu_ein2packet_random) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_container()};
    ulf::mdu_ein::mdu_ein2packet(c);
  }
}

TEST(fuzz, mdu_ein2bytes_random) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_container()};
    ulf::mdu_ein::mdu_ein2bytes(c);
  }
}

TEST(fuzz, mdu_ein2packet_valid) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_packet_frame()};
    auto result{ulf::mdu_ein::mdu_ein2bytes(c)};

    ASSERT_TRUE(result);
    ASSERT_TRUE(*result);
  }
}

TEST(fuzz, mdu_ein2bytes_valid) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_packet_frame()};
    auto result{ulf::mdu_ein::mdu_ein2bytes(c)};

    ASSERT_TRUE(result);
    ASSERT_TRUE(*result);
  }
}
