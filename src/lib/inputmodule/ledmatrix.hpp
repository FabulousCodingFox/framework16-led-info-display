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

  static const int WIDTH = 9;
  static const int HEIGHT = 34;
  static const int PIXELS = WIDTH * HEIGHT;

  /**
   * @brief Tell the firmware to start/stop animation. Scrolls the currently saved grid vertically down.
   * @param device The device handle
   * @param animate Whether to animate or not
   * @return void
   */
  inline void animate(libusb_device_handle* device, bool animate)
  {
    spdlog::trace("Setting integrated animate to {}", animate);
    inputmodule::send_command(device, inputmodule::CommandVals::Animate, {(unsigned char) (animate ? 0x01 : 0x00)});
  }

  /**
   * @brief Get the current animation status
   * @param device The device handle
   * @return The current animation status
   */
  inline auto get_animate(libusb_device_handle* device) -> bool
  {
    spdlog::trace("Getting current animation status");
    auto res = inputmodule::send_command_with_response(device, inputmodule::CommandVals::Animate);
    return res.size() > 0 && res[0] == 0x01;
  }

  /**
   * @brief (Integrated pattern) Sets the brightness of every pixel to maximum
   * @param device The device handle
   * @return void
   */
  inline void pattern_full_brightness(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to full brightness");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::FullBrightness});
  }

  /**
   * @brief (Integrated pattern) Creates a gradient from top to bottom with the brightness ranging from 0% to 13%
   * @param device The device handle
   * @return void
   */
  inline void pattern_gradient(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to gradient");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::Gradient});
  }

  /**
   * @brief (Integrated pattern) Creates a gradient from top to bottom with the brightness starting and ending at 0% and peaking at 7% in the middle
   * @param device The device handle
   * @return void
   */
  inline void pattern_double_gradient(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to double gradient");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DoubleGradient});
  }

  /**
   * @brief (Integrated pattern) Writes "LOTUS" to the led matrix sideways
   * @param device The device handle
   * @return void
   */
  inline void pattern_lotus(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to lotus");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayLotus});
  }

  /**
   * @brief (Integrated pattern) Creates a zigzag pattern
   * @param device The device handle
   * @return void
   */
  inline void pattern_zigzag(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to zigzag");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::ZigZag});
  }

  /**
   * @brief (Integrated pattern) Writes "PANIC" to the led matrix
   * @param device The device handle
   * @return void
   */
  inline void pattern_panic(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to panic");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayPanic});
  }

  /**
   * @brief (Integrated pattern) Writes "LOTUS" to the led matrix top to bottom
   * @param device The device handle
   * @return void
   */
  inline void pattern_lotus2(libusb_device_handle* device)
  {
    spdlog::trace("Setting integrated pattern to lotus2");
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::DisplayLotus2});
  }

  /**
   * @brief (Integrated pattern) Fills up a percentage of the led matrix starting from the bottom
   * @param device The device handle
   * @param value The percentage value to fill up (0-100)
   * @return void
   */
  inline void pattern_percentage(libusb_device_handle* device, unsigned char value)
  {
    spdlog::trace("Setting integrated pattern to percentage with value", value);
    if (value > 100)
    {
      spdlog::error("Value must be between 0 and 100");
      return;
    }
    inputmodule::send_command(device, inputmodule::CommandVals::Pattern, {inputmodule::IntegratedPatternVals::Percentage, value});
  }

  /**
   * @brief Writes a text to the led matrix
   * @param device The device handle
   * @param text The text to write. Cuts off at 5 characters. Must be uppercase
   * @return void
   */
  void pattern_text(libusb_device_handle* device, const std::string& text);

  /**
   * @brief Writes an array of symbol identifiers to the led matrix
   * @param device The device handle
   * @param parts The symbol identifiers to write
   * @return void
   */
  void pattern_symbols(libusb_device_handle* device, std::vector<std::string>& parts);

  /**
   * @brief Lights up a specific number of pixels on the led matrix
   * @param device The device handle
   * @param value The number of pixels to light up
   * @return void
   */
  void pattern_count(libusb_device_handle* device, int value);

  /**
   * @brief Gets the current pwm frequency of the led matrix
   * @param device The device handle
   * @return The pwm frequency in Hz or -1 if the frequency is unknown
   */
  auto get_pwm_freq(libusb_device_handle* device) -> int;

  /**
   * @brief Show a black/white matrix. Send everything in a single command.
   * @param device The device handle
   * @param matrix The matrix to display
   * @return void
   */
  void pattern_matrix(libusb_device_handle* device, std::vector<bool>& matrix);

  /**
   * @brief Display 9 values in equalizer diagram starting from the middle, going up and down
   * @param device The device handle
   * @param values The values to display
   * @return void
   */
  void pattern_equalizer(libusb_device_handle* device, std::vector<unsigned char>& values);

  /**
   * @brief Sets the global brightness of the led matrix
   * @param device The device handle
   * @param value The brightness value (0-255)
   * @return void
   */
  inline void brightness(libusb_device_handle* device, unsigned char value)
  {
    spdlog::trace("Setting global brightness to {}", value);
    inputmodule::send_command(device, inputmodule::CommandVals::Brightness, {value});
  }

  /**
   * @brief Gets the current global brightness of the led matrix
   * @param device The device handle
   * @return The current global brightness
   */
  inline auto get_brightness(libusb_device_handle* device) -> unsigned char
  {
    spdlog::trace("Getting current global brightness");
    auto res = inputmodule::send_command_with_response(device, inputmodule::CommandVals::Brightness);
    return res.size() > 0 ? res[0] : 0;
  }

} // namespace lib::inputmodule::ledmatrix