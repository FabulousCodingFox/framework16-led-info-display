#include "./lib/ledmatrix/ledmatrix.hpp"
#include "./lib/preset.hpp"
#include "./lib/usb/usb.hpp"
#include "./lib/window/window.hpp"
#include "./preset/clock.hpp"
#include "spdlog/spdlog.h"
#include <iostream>
#include <libusb.h>
#include <random>
#include <thread>

auto main(int argc, char* argv[]) -> int
{
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_LEVEL_TRACE));
  spdlog::info("Starting up");

  spdlog::info("Initializing usb library");
  auto handles = lib::usb::init();

  if (handles->empty())
  {
    spdlog::error("No devices found");
    return 1;
  }

  spdlog::info("Found {} devices", handles->size());

  spdlog::info("Registering presets");
  auto presets = new lib::preset::PresetManager();
  presets->addPreset(std::make_shared<preset::clock>());

  return lib::window::run(argc, argv, handles, presets);
}
