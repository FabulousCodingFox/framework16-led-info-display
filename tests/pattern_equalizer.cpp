#include "lib/inputmodule/ledmatrix.hpp"
#include "lib/usb.hpp"
#include "spdlog/spdlog.h"
#include <libusb.h>
#include <random>
#include <vector>

auto main(int argc, const char* argv[]) -> int
{
  spdlog::set_level(spdlog::level::trace);
  std::vector<libusb_device_handle*>* handles = lib::usb::init();
  if (handles->empty())
  {
    spdlog::error("No devices found");
    return 1;
  }

  std::vector<int> weights;
  for (int i = 33; i > 0; --i)
  {
    weights.push_back(i * i);
  }

  // Create the population vector
  std::vector<int> population;
  for (int i = 1; i <= 33; ++i)
  {
    population.push_back(i);
  }

  // Prepare for random sampling with weights
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dist(weights.begin(), weights.end());

  // Generate vals (vector of unsigned chars)
  std::vector<unsigned char> vals;
  for (int i = 0; i < 9; ++i)
  {
    vals.push_back(static_cast<unsigned char>(population[dist(gen)]));
  }

  for (auto handle : *handles)
  {
    lib::inputmodule::ledmatrix::pattern_equalizer(handle, vals);
  }
  lib::usb::exit();
}