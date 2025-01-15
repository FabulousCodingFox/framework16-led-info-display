#pragma once

#include "./ledmatrix.hpp"
#include <array>
#include <string>

namespace lib::inputmodule::font
{
  static const int WIDTH = 5;
  static const int HEIGHT = 6;

  auto get_char(std::string& c) -> const std::array<bool, 30>&;
} // namespace lib::inputmodule::font