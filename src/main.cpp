#include "lib/inputmodule/ledmatrix.hpp"
#include "lib/usb.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <libusb.h>
#include <thread>

auto main(int argc, const char* argv[]) -> int
{
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_LEVEL_TRACE));
  spdlog::info("Starting up");

  spdlog::info("Initializing usb library");
  std::vector<libusb_device_handle*>* handles = lib::usb::init();

  if (handles->empty())
  {
    spdlog::error("No devices found");
    return 1;
  }

  spdlog::info("Found {} devices", handles->size());

  for (auto handle : *handles)
  {
    std::vector<std::string> parts{"!!", "cloud", "rain", ":)", "batteryLow"};
    lib::inputmodule::ledmatrix::pattern_symbols(handle, parts);
    lib::inputmodule::ledmatrix::integrated_animate(handle, false);

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  lib::usb::exit();

  return 0;
}
