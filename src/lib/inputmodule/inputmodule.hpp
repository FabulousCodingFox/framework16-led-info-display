#pragma once

#include <libusb.h>
#include <vector>

namespace lib::inputmodule::inputmodule
{
  enum CommandVals : unsigned char
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

  enum IntegratedPatternVals : unsigned char
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

  void send_command(libusb_device_handle* device, unsigned char command, const std::vector<unsigned char>& parameters = {});
  auto send_command_with_response(libusb_device_handle* device, unsigned char command, const std::vector<unsigned char>& parameters = {}) -> std::vector<unsigned char>;
} // namespace lib::inputmodule::inputmodule