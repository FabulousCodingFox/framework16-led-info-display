#pragma once

#include "../ledmatrix/ledmatrix.hpp"
#include <vector>

namespace lib::usb
{
  auto init() -> std::vector<ledmatrix::ledmatrix*>*;
  void exit();
} // namespace lib::usb