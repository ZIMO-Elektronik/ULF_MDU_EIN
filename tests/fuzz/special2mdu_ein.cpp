#include <gtest/gtest.h>
#include <ulf/mdu_ein.hpp>
#include "random_container.hpp"

TEST(fuzz, special2mdu_ein_random) {
  std::random_device rd{};
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_container(16, 16)};
    std::span<uint8_t const, 16uz> t{c};
    ulf::mdu_ein::special2mdu_ein(
      static_cast<ulf::mdu_ein::Command>(rd()), static_cast<uint8_t>(rd()), t);
  }
}
