#pragma once

#include <libusb.h>
#include <vector>

namespace lib::usb
{
  auto init() -> std::vector<libusb_device_handle*>*;
  void exit();
} // namespace lib::usb