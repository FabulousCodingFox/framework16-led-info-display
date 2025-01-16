#pragma once

#include "../lib/preset.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace preset
{
  class clock : public lib::preset::IPreset
  {
  public:
    std::string get_name() override { return "Clock"; };
    std::string get_id() override { return "clock"; };
    void init(lib::ledmatrix::ledmatrix& device) override {};
    void render(lib::ledmatrix::ledmatrix& device, lib::preset::RenderOptions& options) override
    {
      // Get the current time
      auto now = std::chrono::system_clock::now();
      auto now_time_t = std::chrono::system_clock::to_time_t(now);

      // Convert to local time
      std::tm local_time;
#if defined(_MSC_VER) // For Windows
      localtime_s(&local_time, &now_time_t);
#else // For POSIX systems
      localtime_r(&now_time_t, &local_time);
#endif

      // Format the time as HH:MM
      std::ostringstream time_stream;
      time_stream << std::put_time(&local_time, "%H:%M");
      std::string current_time = time_stream.str();

      device.pattern_text(current_time);
    };
    void exit(lib::ledmatrix::ledmatrix& device) override {};
  };
} // namespace preset