#include "lib/inputmodule/ledmatrix.hpp"
#include "lib/usb.hpp"
#include "spdlog/spdlog.h"
#include <libusb.h>

auto main(int argc, const char* argv[]) -> int
{
  spdlog::set_level(spdlog::level::trace);
  std::vector<libusb_device_handle*>* handles = lib::usb::init();
  if (handles->empty())
  {
    spdlog::error("No devices found");
    return 1;
  }
  for (auto handle : *handles)
  {
    lib::inputmodule::ledmatrix::pattern_percentage(handle, 39);
  }
  lib::usb::exit();
}