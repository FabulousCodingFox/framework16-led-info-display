#include "lib/inputmodule/ledmatrix.hpp"
#include "lib/usb.hpp"
#include "spdlog/spdlog.h"
#include <libusb.h>
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
  std::vector<std::string> parts = {"heart", "batteryLow", "C", "cloud", ":)"};
  for (auto handle : *handles)
  {
    lib::inputmodule::ledmatrix::pattern_symbols(handle, parts);
  }
  lib::usb::exit();
}