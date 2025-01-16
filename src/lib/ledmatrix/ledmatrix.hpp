#pragma once

#include <cstdint>
#include <libusb.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace lib::ledmatrix
{
  enum command : uint8_t
  {
    Brightness = 0x00,
    Pattern = 0x01,
    BootloaderReset = 0x02,
    Sleep = 0x03,
    Animate = 0x04,
    Panic = 0x05,
    Draw = 0x06,
    StageGreyCol = 0x07,
    DrawGreyColBuffer = 0x08,
    SetText = 0x09,
    StartGame = 0x10,
    GameControl = 0x11,
    GameStatus = 0x12,
    SetColor = 0x13,
    DisplayOn = 0x14,
    InvertScreen = 0x15,
    SetPixelColumn = 0x16,
    FlushFramebuffer = 0x17,
    ClearRam = 0x18,
    ScreenSaver = 0x19,
    SetFps = 0x1A,
    SetPowerMode = 0x1B,
    PwmFreq = 0x1E,
    DebugMode = 0x1F,
    Version = 0x20,
  };

  enum integrated_pattern : uint8_t
  {
    Percentage = 0x00,
    Gradient = 0x01,
    DoubleGradient = 0x02,
    DisplayLotus = 0x03,
    ZigZag = 0x04,
    FullBrightness = 0x05,
    DisplayPanic = 0x06,
    DisplayLotus2 = 0x07,
  };

  static const unsigned int VID = 0x32AC;
  static const unsigned int PID = 0x0020;

  static const int WIDTH = 9;
  static const int HEIGHT = 34;
  static const int PIXELS = WIDTH * HEIGHT;

  class ledmatrix
  {
  public:
    libusb_device_handle* device;

    ledmatrix(libusb_device_handle* device);
    ~ledmatrix();

    void send_command(command command, const std::vector<uint8_t>& parameters = {});
    auto send_command_with_response(command command, const std::vector<uint8_t>& parameters = {}) -> std::vector<uint8_t>;

    /**
     * @brief Tell the firmware to start/stop animation. Scrolls the currently saved grid vertically down.
     * @param device The device handle
     * @param animate Whether to animate or not
     * @return void
     */
    inline void animate(bool animate = true)
    {
      spdlog::trace("Setting integrated animate to {}", animate);
      this->send_command(command::Animate, {(uint8_t) (animate ? 0x01 : 0x00)});
    }

    /**
     * @brief Get the current animation status
     * @param device The device handle
     * @return The current animation status
     */
    inline auto get_animate() -> bool
    {
      spdlog::trace("Getting current animation status");
      auto res = this->send_command_with_response(command::Animate);
      return res.size() > 0 && res[0] == 0x01;
    }

    /**
     * @brief (Integrated pattern) Sets the brightness of every pixel to maximum
     * @param device The device handle
     * @return void
     */
    inline void pattern_full_brightness()
    {
      spdlog::trace("Setting integrated pattern to full brightness");
      this->send_command(command::Pattern, {integrated_pattern::FullBrightness});
    }

    /**
     * @brief (Integrated pattern) Creates a gradient from top to bottom with the brightness ranging from 0% to 13%
     * @param device The device handle
     * @return void
     */
    inline void pattern_gradient()
    {
      spdlog::trace("Setting integrated pattern to gradient");
      this->send_command(command::Pattern, {integrated_pattern::Gradient});
    }

    /**
     * @brief (Integrated pattern) Creates a gradient from top to bottom with the brightness starting and ending at 0% and peaking at 7% in the middle
     * @param device The device handle
     * @return void
     */
    inline void pattern_double_gradient()
    {
      spdlog::trace("Setting integrated pattern to double gradient");
      this->send_command(command::Pattern, {integrated_pattern::DoubleGradient});
    }

    /**
     * @brief (Integrated pattern) Writes "LOTUS" to the led matrix sideways
     * @param device The device handle
     * @return void
     */
    inline void pattern_lotus()
    {
      spdlog::trace("Setting integrated pattern to lotus");
      this->send_command(command::Pattern, {integrated_pattern::DisplayLotus});
    }

    /**
     * @brief (Integrated pattern) Creates a zigzag pattern
     * @param device The device handle
     * @return void
     */
    inline void pattern_zigzag()
    {
      spdlog::trace("Setting integrated pattern to zigzag");
      this->send_command(command::Pattern, {integrated_pattern::ZigZag});
    }

    /**
     * @brief (Integrated pattern) Writes "PANIC" to the led matrix
     * @param device The device handle
     * @return void
     */
    inline void pattern_panic()
    {
      spdlog::trace("Setting integrated pattern to panic");
      this->send_command(command::Pattern, {integrated_pattern::DisplayPanic});
    }

    /**
     * @brief (Integrated pattern) Writes "LOTUS" to the led matrix top to bottom
     * @param device The device handle
     * @return void
     */
    inline void pattern_lotus2()
    {
      spdlog::trace("Setting integrated pattern to lotus2");
      this->send_command(command::Pattern, {integrated_pattern::DisplayLotus2});
    }

    /**
     * @brief (Integrated pattern) Fills up a percentage of the led matrix starting from the bottom
     * @param device The device handle
     * @param value The percentage value to fill up (0-100)
     * @return void
     */
    inline void pattern_percentage(uint8_t value)
    {
      spdlog::trace("Setting integrated pattern to percentage with value", value);
      if (value > 100)
      {
        spdlog::error("Value must be between 0 and 100");
        return;
      }
      this->send_command(command::Pattern, {integrated_pattern::Percentage, value});
    }

    /**
     * @brief Writes a text to the led matrix
     * @param device The device handle
     * @param text The text to write. Cuts off at 5 characters. Must be uppercase
     * @return void
     */
    void pattern_text(const std::string& text);

    /**
     * @brief Writes an array of symbol identifiers to the led matrix
     * @param device The device handle
     * @param parts The symbol identifiers to write
     * @return void
     */
    void pattern_symbols(std::vector<std::string>& parts);

    /**
     * @brief Lights up a specific number of pixels on the led matrix
     * @param device The device handle
     * @param value The number of pixels to light up
     * @return void
     */
    void pattern_count(int value);

    /**
     * @brief Gets the current pwm frequency of the led matrix
     * @param device The device handle
     * @return The pwm frequency in Hz or -1 if the frequency is unknown
     */
    auto get_pwm_freq() -> int;

    /**
     * @brief Show a black/white matrix. Send everything in a single command.
     * @param device The device handle
     * @param matrix The matrix to display
     * @return void
     */
    void pattern_matrix(std::vector<bool>& matrix);

    /**
     * @brief Display 9 values in equalizer diagram starting from the middle, going up and down
     * @param device The device handle
     * @param values The values to display
     * @return void
     */
    void pattern_equalizer(std::vector<uint8_t>& values);

    /**
     * @brief Sets the global brightness of the led matrix
     * @param device The device handle
     * @param value The brightness value (0-255)
     * @return void
     */
    inline void brightness(uint8_t value)
    {
      spdlog::trace("Setting global brightness to {}", value);
      this->send_command(command::Brightness, {value});
    }

    /**
     * @brief Gets the current global brightness of the led matrix
     * @param device The device handle
     * @return The current global brightness
     */
    inline auto get_brightness() -> uint8_t
    {
      spdlog::trace("Getting current global brightness");
      auto res = this->send_command_with_response(command::Brightness);
      return res.size() > 0 ? res[0] : 0;
    }
  };
} // namespace lib::ledmatrix