#include <gtest/gtest.h>
#include <ulf/mdu_ein.hpp>
#include "random_container.hpp"

TEST(fuzz, packet2mdu_ein_random) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_container(1uz, MDU_MAX_PACKET_SIZE)};
    ulf::mdu_ein::bytes2mdu_ein(c);
  }
}
