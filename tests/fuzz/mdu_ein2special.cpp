#include <gtest/gtest.h>
#include <ulf/mdu_ein.hpp>
#include "random_container.hpp"

TEST(fuzz, mdu_ein2special_random) {
  for (auto i{0uz}; i < FUZZ_REPETITIONS; ++i) {
    auto c{random_container()};
    ulf::mdu_ein::detail::mdu_ein2special(c);
  }
}
