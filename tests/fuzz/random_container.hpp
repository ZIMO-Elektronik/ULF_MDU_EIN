#pragma once

#include <algorithm>
#include <random>
#include <vector>

inline std::vector<uint8_t> random_container(size_t min_length = 1,
                                             size_t max_length = 1024) {
  std::random_device rd{};
  std::vector<uint8_t> result{};

  // Create a container with random length and fill with random elements
  size_t length{rd() % max_length};
  if (length < min_length) length = min_length;
  result.resize(length);

  std::generate(begin(result), end(result), [&rd]() { return rd(); });
  return result;
}

inline std::vector<uint8_t> random_packet_frame(size_t min_length = 1,
                                                size_t max_length = 1024) {
  auto c{random_container(min_length, max_length)};
  std::vector<uint8_t> result{};

  auto iter{std::back_inserter(result)};

  std::ranges::copy(ulf::mdu_ein::prefix, iter);                    // Prefix
  ulf::mdu_ein::uint16_2data(static_cast<uint16_t>(size(c)), iter); // Length
  std::ranges::copy(c, iter);                                       // Data
  std::ranges::copy(ulf::mdu_ein::suffix, iter);                    // Suffix

  return result;
}
