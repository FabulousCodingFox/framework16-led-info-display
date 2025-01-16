#pragma once

#include "./ledmatrix/ledmatrix.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace lib::ledmatrix
{
  class ledmatrix;
}

namespace lib::preset
{
  class IPreset
  {
  public:
    virtual ~IPreset() = default;

    virtual std::string get_name() = 0;
    virtual std::string get_id() = 0;

    virtual void init(lib::ledmatrix::ledmatrix* device) = 0;
    virtual void render(lib::ledmatrix::ledmatrix* device) = 0;
    virtual void exit(lib::ledmatrix::ledmatrix* device) = 0;
    virtual std::shared_ptr<IPreset> clone() const = 0;
  };

  class PresetManager
  {
  private:
    std::map<std::string, std::shared_ptr<IPreset>> presets;

  public:
    void add(const std::shared_ptr<IPreset>& preset)
    {
      presets[preset->get_id()] = preset;
    }
    auto list() -> std::vector<std::shared_ptr<IPreset>>
    {
      std::vector<std::shared_ptr<IPreset>> result;
      for (const auto& [_, preset] : presets)
      {
        result.push_back(preset);
      }
      return result;
    }
    auto get(const std::string& id) -> std::shared_ptr<IPreset>
    {
      auto it = presets.find(id);
      if (it != presets.end())
      {
        return it->second;
      }
      return nullptr;
    }
  };
} // namespace lib::preset
