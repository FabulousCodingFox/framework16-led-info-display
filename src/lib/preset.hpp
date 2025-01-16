#pragma once

#include "./ledmatrix/ledmatrix.hpp"
#include <cstdint>
#include <map>
#include <string>

namespace lib::preset
{
  struct RenderOptions
  {
    uint8_t fps;
    uint8_t brightness;
  };

  class IPreset
  {
  public:
    virtual ~IPreset() = default;

    virtual std::string get_name() = 0;
    virtual std::string get_id() = 0;

    virtual void init(lib::ledmatrix::ledmatrix& device) = 0;
    virtual void render(lib::ledmatrix::ledmatrix& device, RenderOptions& options) = 0;
    virtual void exit(lib::ledmatrix::ledmatrix& device) = 0;
  };

  class PresetManager
  {
  private:
    std::map<std::string, std::shared_ptr<IPreset>> presets;

  public:
    void addPreset(const std::shared_ptr<IPreset>& preset)
    {
      presets[preset->get_id()] = preset;
    }
  };
} // namespace lib::preset
