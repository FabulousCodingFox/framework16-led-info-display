#pragma once

#include "./inputmodule.hpp"
#include "font.hpp"
#include "spdlog/spdlog.h"
#include <functional>
#include <libusb.h>
#include <string>

namespace lib::inputmodule::ledmatrix
{
  static const unsigned int VID = 0x32AC;
  static const unsigned int PID = 0x0020;

  static const unsigned int WIDTH = 9;
  static const unsigned int HEIGHT = 34;
  static const unsigned int PIXELS = WIDTH * HEIGHT;

  inline void integrated_animate(libusb_device_handle* device, bool animate)
  {
    spdlog::trace("Setting integrated animate to {}", animate);
    inputmodule::send_command(device, inputmodule::CommandVals::Animate, {(unsigned char) (animate ? 0x01 : 0x00)});
  }

  inline void pattern_full_brightness(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to full brightness");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::FullBrightness});
  }

  inline void pattern_gradient(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to gradient");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::Gradient});
  }

  inline void pattern_double_gradient(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to double gradient");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DoubleGradient});
  }

  inline void pattern_lotus(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to lotus");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayLotus});
  }

  inline void pattern_zigzag(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to zigzag");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::ZigZag});
  }

  inline void pattern_panic(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to panic");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayPanic});
  }

  inline void pattern_lotus2(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to lotus2");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayLotus2});
  }

  void pattern_text(libusb_device_handle* device, const std::string& text);

  void pattern_symbols(libusb_device_handle* device, std::vector<std::string>& parts);
} // namespace lib::inputmodule::ledmatrix