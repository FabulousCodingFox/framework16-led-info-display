#include "./font.hpp"
#include <array>
#include <map>
#include <string>

namespace lib::inputmodule::font
{
  static const std::map<std::string, std::array<bool, 30>>* FONT_MAP = new std::map<std::string, std::array<bool, 30>>{
      {"0", {false, true, true, false, false, true, false, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, false, true, false, false, true, true, false, false}},
      {"1", {false, false, true, false, false, false, true, true, false, false, true, false, true, false, false, false, false, true, false, false, false, false, true, false, false, true, true, true, true, true}},
      {"2", {true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true}},
      {"3", {true, true, true, true, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true, false}},
      {"4", {false, false, false, true, false, false, false, true, true, false, false, true, false, true, false, true, true, true, true, true, false, false, false, true, false, false, false, false, true, false}},
      {"5", {true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true, false}},
      {"6", {false, true, true, true, false, true, false, false, false, false, true, true, true, true, true, true, false, false, false, true, true, false, false, false, true, false, true, true, true, false}},
      {"7", {true, true, true, true, true, false, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false}},
      {"8", {false, true, true, true, false, true, false, false, false, true, false, true, true, true, false, true, false, false, false, true, true, false, false, false, true, false, true, true, true, false}},
      {"9", {false, true, true, true, false, true, false, false, false, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, true, true, true, false}},
      {":", {false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false}},
      {" ", {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}},
      {"?", {false, true, true, false, false, false, false, false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, false, false, false, false, true, false, false}},
      {".", {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false}},
      {",", {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false}},
      {"!", {false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, false, false, false, false, false, true, false, false}},
      {"/", {false, false, false, false, true, false, false, false, true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, false, false, true, false, false, false, false}},
      {"*", {false, false, false, false, false, false, true, false, true, false, false, false, true, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false}},
      {"%", {true, true, false, false, true, true, true, false, true, true, false, false, true, true, false, false, true, true, false, false, true, true, false, true, true, true, false, false, true, true}},
      {"+", {false, false, true, false, false, false, false, true, false, false, true, true, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false, false, false, false}},
      {"-", {false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}},
      {"=", {false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false}},
      {"A", {false, true, true, true, false, true, false, false, false, true, true, true, true, true, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true}},
      {"B", {true, true, true, true, false, true, false, false, false, true, true, true, true, true, false, true, false, false, false, true, true, false, false, false, true, true, true, true, true, false}},
      {"C", {true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true}},
      {"D", {true, true, true, true, false, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, true, true, true, false}},
      {"E", {true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true}},
      {"F", {true, true, true, true, true, true, false, false, false, false, true, true, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false}},
      {"G", {false, true, true, true, false, true, false, false, false, false, true, false, true, true, true, true, false, false, false, true, true, false, false, false, true, false, true, true, true, false}},
      {"H", {true, false, false, false, true, true, false, false, false, true, true, true, true, true, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true}},
      {"I", {false, true, true, true, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, true, true, true, false}},
      {"J", {false, true, true, true, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, true, false, false, true, false, false, true, true, false}},
      {"K", {true, false, false, true, false, true, false, true, false, false, true, true, false, false, false, true, false, true, false, false, true, false, false, true, false, true, false, false, false, true}},
      {"L", {true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, true, true, true, true}},
      {"M", {false, false, false, false, false, false, true, false, true, false, true, false, true, false, true, true, false, true, false, true, true, false, true, false, true, true, false, true, false, true}},
      {"N", {true, false, false, false, true, true, true, false, false, true, true, false, true, false, true, true, false, true, false, true, true, false, true, false, true, true, false, false, true, true}},
      {"O", {false, true, true, true, false, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, false, true, true, true, false}},
      {"P", {true, true, true, false, false, true, false, false, true, false, true, false, false, true, false, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false}},
      {"Q", {false, true, true, true, false, true, false, false, false, true, true, false, false, false, true, true, false, true, false, true, true, false, false, true, false, false, true, true, false, true}},
      {"R", {true, true, true, true, false, true, false, false, true, false, true, true, true, true, false, true, true, false, false, false, true, false, true, false, false, true, false, false, true, false}},
      {"S", {true, true, true, true, true, true, false, false, false, false, false, true, true, true, false, false, false, false, false, true, false, false, false, false, true, true, true, true, true, false}},
      {"T", {true, true, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false}},
      {"U", {true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, true, true, true, true}},
      {"V", {true, false, false, false, true, true, false, false, false, true, false, true, false, true, true, false, true, false, true, true, false, false, true, false, false, false, false, true, false, false}},
      {"W", {true, false, false, false, true, true, false, false, false, true, true, false, true, false, true, true, false, true, false, true, false, true, false, true, false, false, true, false, true, false}},
      {"X", {true, false, false, false, true, false, true, false, true, false, false, false, true, false, false, false, false, true, false, false, false, true, false, true, false, true, false, false, false, true}},
      {"Y", {true, false, false, false, true, true, false, false, false, true, false, true, false, true, false, false, true, false, true, false, false, false, true, false, false, false, false, true, false, false}},
      {"Z", {true, true, true, true, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false, false, true, true, true, true, true}},
      {"Ä", {false, true, false, true, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, true, true, true, true, true, true, true, false, false, false, true}},
      {"Ö", {false, true, false, true, false, false, false, false, false, false, false, true, true, true, false, true, false, false, false, true, true, false, false, false, true, false, true, true, true, false}},
      {"Ü", {false, true, false, true, false, false, false, false, false, false, true, false, false, false, true, true, false, false, false, true, true, false, false, false, true, true, true, true, true, true}},
      {"degC", {true, true, false, false, false, true, true, false, false, false, false, false, true, true, true, false, false, true, false, false, false, false, true, false, false, false, false, true, true, true}},
      {"degF", {true, true, false, false, false, true, true, false, false, false, false, false, true, true, true, false, false, true, false, false, false, false, true, true, true, false, false, true, false, false}},
      {"snow", {false, false, false, false, false, true, false, true, false, true, false, true, true, true, false, true, true, true, true, true, false, true, true, true, false, true, false, true, false, true}},
      {"sun", {false, false, false, false, false, false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true, false}},
      {"cloud", {false, false, false, false, false, false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false}},
      {"rain", {false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, false, true, false, false, true, false, false, true, false, false, true, false, false, true, false}},
      {"thunder", {false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, false, false, true, false, false, false, true, false, false, false, false, false, true, false, false}},
      {"batteryLow", {false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, true, false, false, true, true, true, false, false, true, true, true, true, true, true, false}},
      {"!!", {false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, false, false, false, false, false, false, true, false, true, false, false, true, false, true, false}},
      {"heart", {false, false, false, false, false, true, true, false, true, true, true, true, true, true, true, false, true, true, true, false, false, false, true, false, false, false, false, false, false, false}},
      {"heart0", {true, true, false, true, true, true, true, true, true, true, false, true, true, true, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false}},
      {"heart2", {false, false, false, false, false, false, false, false, false, false, true, true, false, true, true, true, true, true, true, true, false, true, true, true, false, false, false, true, false, false}},
      {":)", {false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, false, true, true, true, false}},
      {":|", {false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false}},
      {":(", {false, false, false, false, false, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, true, false, false, false, true}},
      {";)", {false, false, false, false, false, true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, false, true, true, true, false}}};

  auto get_char(std::string& c) -> const std::array<bool, 30>&
  {
    auto it = FONT_MAP->find(c);
    if (it != FONT_MAP->end())
    {
      return it->second;
    }
    else
    {
      return FONT_MAP->at("?");
    }
  }
} // namespace lib::inputmodule::font